#pragma once

#include <memory>
#include <type_traits>

// from https://github.com/joboccara/NamedType
// Enable empty base class optimization with multiple inheritance on Visual
// Studio.
#if defined(_MSC_VER) && _MSC_VER >= 1910
#define STORM_EBCO __declspec(empty_bases)
#else
#define STORM_EBCO
#endif

namespace storm::core {
	template <typename T>
	using IsNotReference =
	    typename std::enable_if<!std::is_reference<T>::value, void>::type;

	class NamedTypeTrait {};
	template <typename T, typename Parameter,
	    template <typename> class... Skills>
	class STORM_EBCO NamedType
	    : public NamedTypeTrait
	    , public Skills<NamedType<T, Parameter, Skills...>>... {
	public:
		using UnderlyingType = T;

		template <typename... Args>
		explicit constexpr NamedType(Args &&... args)
		    : value_(std::forward<Args>(args)...) {}
		explicit constexpr NamedType(T const &value) : value_(value) {}
		template <typename T_ = T, typename = IsNotReference<T_>>
		explicit constexpr NamedType(T &&value) : value_(std::move(value)) {}

		constexpr T &      get() { return value_; }
		constexpr T const &get() const { return value_; }

		using ref = NamedType<T &, Parameter, Skills...>;
		operator ref() { return ref(value_); }

		struct Argument {
			template <typename U>
			NamedType operator=(U &&value) const {
				return NamedType(std::forward<U>(value));
			}
			Argument()                 = default;
			Argument(Argument const &) = delete;
			Argument(Argument &&)      = delete;
			Argument &operator=(Argument const &) = delete;
			Argument &operator=(Argument &&) = delete;
		};

	private:
		T value_;
	};

	template <class StrongType, typename... Args>
	constexpr StrongType makeNamed(Args &&... args) {
		return StrongType(
		    typename StrongType::UnderlyingType(std::forward<Args>(args)...));
	}
}

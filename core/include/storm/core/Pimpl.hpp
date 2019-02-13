#pragma once

#include <memory>
#include <utility>

namespace storm::core {
	template <typename T, bool initialize = true>
	class Pimpl {
	public:
		Pimpl();

		template <typename... Args>
		Pimpl(Args &&... args);

		template <typename... Args>
		inline void init(Args &&... args);

		inline T *      operator->() noexcept;
		inline T const *operator->() const noexcept;

		inline T &      operator*() noexcept;
		inline T const &operator*() const noexcept;

		inline T *            get() noexcept;
		inline const T *const get() const noexcept;

	private:
		std::unique_ptr<T> m_ptr;
	};
}

#include "Pimpl.tpp"

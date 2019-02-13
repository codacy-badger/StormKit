// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <type_traits>

namespace storm::core {
	template <class... Ts>
	struct overload : Ts... {
		using Ts::operator()...;
	};
	template <class... Ts>
	overload(Ts...)->overload<Ts...>;

	struct nonesuch {
		~nonesuch()                = delete;
		nonesuch(nonesuch const &) = delete;
		void operator=(nonesuch const &) = delete;
	};

	namespace _private {
		template <class Default, class AlwaysVoid, template <class...> class Op,
		    class... Args>
		struct detector {
			using value_t = std::false_type;
			using type    = Default;
		};

		template <class Default, template <class...> class Op, class... Args>
		struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
			using value_t = std::true_type;
			using type    = Op<Args...>;
		};
	}

	template <template <class...> class Op, class... Args>
	using detected_t =
	    typename _private::detector<nonesuch, void, Op, Args...>::type;

	template <class Default, template <class...> class Op, class... Args>
	using detected_or = _private::detector<Default, void, Op, Args...>;

	template <template <class...> class Op, class... Args>
	using is_detected =
	    typename _private::detector<nonesuch, void, Op, Args...>::value_t;

	template <template <class...> class Op, class... Args>
	constexpr auto is_detected_v = is_detected<Op, Args...>::value;

	template <class Default, template <class...> class Op, class... Args>
	using detected_or_t = typename detected_or<Default, Op, Args...>::type;

	template <class Expected, template <class...> class Op, class... Args>
	using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

	template <class Expected, template <class...> class Op, class... Args>
	constexpr bool is_detected_exact_v
	    = is_detected_exact<Expected, Op, Args...>::value;

	template <class To, template <class...> class Op, class... Args>
	using is_detected_convertible
	    = std::is_convertible<detected_t<Op, Args...>, To>;

	template <class To, template <class...> class Op, class... Args>
	constexpr bool is_detected_convertible_v
	    = is_detected_convertible<To, Op, Args...>::value;
}

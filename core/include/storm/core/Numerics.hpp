#pragma once
/**
 * @file NumericHelpers.hpp
 * @brief Numeric tools
 * @author Arthapz
 * @version 0.1
 **/

#include <array>
#include <cfloat>
#include <functional>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace storm::core {
	namespace _private {
		static std::default_random_engine generator {};
	}

	/**
	 * @brief init the random seed
	 *
	 * Function wich initialize the seed for the random generator
	 *
	 * @param number : the seed (std::uint64_t)
	 */
	inline void seed(std::uint32_t seed) { _private::generator.seed(seed); }

	/**
	 * @brief get a random number
	 *
	 * Function wich generate a floating point number borned bt [min, max[
	 *
	 * @param min : the min of the value (templated)
	 * @param min : the max of the value (templated)
	 * @return the random number borned by min and max
	 */
	template <typename T>
	auto rand(T &&min, T &&max) {
		using Type = std::decay_t<std::remove_cv_t<T>>;
		if constexpr (std::is_floating_point_v<Type>) {
			std::uniform_real_distribution<Type> dis(min, max);
			return dis(_private::generator);
		} else {
			std::uniform_int_distribution<Type> dis(min, max);
			return dis(_private::generator);
		}
	}

	template <typename T, typename V>
	inline constexpr std::enable_if_t<
	    std::is_floating_point_v<std::decay_t<std::remove_cv_t<
	        V>>> && std::is_floating_point_v<std::decay_t<std::remove_cv_t<T>>>,
	    bool>
	realIsEqual(T &&a, V &&b) noexcept {
		return std::abs(a - b) < std::numeric_limits<
		           std::remove_cv_t<std::decay_t<T>>>::epsilon();
	}

	template <typename T>
	inline constexpr T map(T x, T in_min, T in_max, T out_min, T out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	template <typename T>
	static constexpr const T pi = T(
	    3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211L);

	static constexpr const auto piLD = pi<long double>;
	static constexpr const auto piD  = pi<double>;
	static constexpr const auto piF  = pi<float>;
	static constexpr const auto piI  = pi<std::int32_t>;

	/**
	 * @brief convert degree to radian
	 *
	 * @param angle : angle value in degree (T)
	 *
	 * @return angle in radian
	 */
	template <typename T>
	inline constexpr float degToRad(T angle) noexcept {
		return angle / 180. * pi<T>;
	}

	/**
	 * @brief convert degree to radian
	 *
	 * @param angle : angle value in radian (T)
	 *
	 * @return angle in degree
	 */
	template <typename T>
	inline constexpr T radToDeg(T angle) noexcept {
		return angle / pi<T> * 180.;
	}

	template <class Integer, Integer start, Integer stop>
	constexpr decltype(auto) genRange() {
		std::array<Integer, stop - start> range;
		std::iota(std::begin(range), std::end(range), start);
		return range;
	}

	template <class Integer, class Integer2>
	constexpr decltype(auto) genRange(Integer start, Integer2 stop) {
		std::vector<decltype(start + stop)> range;
		range.resize(stop - start);
		std::iota(std::begin(range), std::end(range), start);
		return range;
	}

	template <class T>
	inline std::intptr_t extractPtr(T *ptr) {
		return reinterpret_cast<std::intptr_t>(ptr);
	}
}

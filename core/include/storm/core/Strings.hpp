#pragma once

#include <functional>
#include <iomanip>
#include <locale>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <storm/core/Assert.hpp>
#include <storm/core/TypeTraits.hpp>
#include <string>

#ifdef STORM_GLM_CONVERSION
#include <glm/gtx/string_cast.hpp>
#endif

namespace storm::core {
	inline std::vector<std::string_view> split(
	    std::string_view string, char delim) {
		auto output = std::vector<std::string_view> {};
		auto first  = size_t {0u};

		while (first < string.size()) {
			const auto second = string.find_first_of(delim, first);

			if (first != second)
				output.emplace_back(string.substr(first, second - first));

			if (second == std::string_view::npos)
				break;

			first = second + 1;
		}

		return output;
	}

	template <typename T, bool complete = true>
	std::string intToHex(T i) {
		static_assert(std::is_integral<T>::value,
		    "Template argument 'T' must be a fundamental integer type (e.g. "
		    "int, short, etc..).");

		auto stream = std::stringstream {};

		if constexpr (complete)
			stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2u)
			       << std::hex;
		else
			stream << std::hex;

		if constexpr (std::is_same_v<std::uint8_t, T>)
			stream << static_cast<std::int32_t>(i);
		else if constexpr (std::is_same_v<std::int8_t, T>)
			stream << static_cast<std::int32_t>(static_cast<uint8_t>(i));
		else
			stream << i;

		return stream.str();
	}

	/**
	 * @brief convert std::string to T
	 *
	 * Function wich a std::string to a T
	 *
	 * @param str : the string which be converted
	 * @return the converted value
	 */
	template <typename T>
	inline T fromString(const std::string &str) {
		auto ss = std::istringstream {str};

		T x {};
		ss >> x;

		return x;
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline std::int8_t fromString(const std::string &str) {
		return static_cast<std::int8_t>(std::stoi(str));
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline std::int32_t fromString(const std::string &str) {
		return std::stoi(str);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline std::int64_t fromString(const std::string &str) {
		return std::stoll(str);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline std::uint8_t fromString(const std::string &str) {
		return static_cast<std::uint8_t>(std::stoi(str));
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline std::uint32_t fromString(const std::string &str) {
		return static_cast<std::uint32_t>(std::stoul(str));
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline std::uint64_t fromString(const std::string &str) {
		return std::stoull(str);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline float fromString(const std::string &str) {
		return std::stof(str);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline double fromString(const std::string &str) {
		return std::stod(str);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <>
	inline long double fromString(const std::string &str) {
		return std::stold(str);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(const char *str) { return str; }

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::string &&str) { return str; }

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(const std::string &str) { return str; }

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::int8_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::uint8_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::int16_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::uint16_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::int32_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::uint32_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::int64_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(std::uint64_t ptr) {
		return std::to_string(ptr);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
	inline std::string to_string(T ptr) {
		return intToHex<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(ptr));
	}
#ifdef STORM_GLM_CONVERSION
	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::mat4 &&mat) {
		return glm::to_string(mat);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(const glm::mat4 &mat) {
		return glm::to_string(mat);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::mat4 &mat) { return glm::to_string(mat); }

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::vec2 &&vec) {
		return glm::to_string(vec);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(const glm::vec2 &vec) {
		return glm::to_string(vec);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::vec2 &vec) { return glm::to_string(vec); }

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::vec3 &&vec) {
		return glm::to_string(vec);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(const glm::vec3 &vec) {
		return glm::to_string(vec);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::vec3 &vec) { return glm::to_string(vec); }

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::vec4 &&vec) {
		return glm::to_string(vec);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(const glm::vec4 &vec) {
		return glm::to_string(vec);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	inline std::string to_string(glm::vec4 &vec) { return glm::to_string(vec); }
#endif
	template <typename String = std::string>
	inline auto to_lower(
	    String &&string, std::locale locale = std::locale("")) {
		using StringRawType = std::remove_reference_t<String>;

		auto &f
		    = std::use_facet<std::ctype<typename StringRawType::value_type>>(
		        locale);

		auto result = StringRawType {std::forward<String>(string)};
		f.tolower(&result[0], &result[0] + result.size());

		return result;
	}

	template <typename String = std::string>
	inline String to_upper(
	    String &&string, std::locale locale = std::locale("")) {
		using StringRawType = std::remove_reference_t<String>;

		auto &f
		    = std::use_facet<std::ctype<typename StringRawType::value_type>>(
		        locale);

		auto result = StringRawType {std::forward<String>(string)};
		f.toupper(&result[0], &result[0] + result.size());

		return result;
	}

	template <typename T>
	using std_to_string_expression
	    = decltype(std::to_string(std::declval<T>()));

	template <typename T>
	constexpr auto has_std_to_string
	    = is_detected_v<std_to_string_expression, T>;

	template <typename T>
	using to_string_expression = decltype(to_string(std::declval<T>()));

	template <typename T>
	constexpr auto has_to_string = is_detected_v<to_string_expression, T>;

	template <typename T>
	using member_to_string_expression = decltype(std::declval<T>().toString());

	template <typename T>
	constexpr auto has_member_to_string
	    = is_detected_v<std_to_string_expression, T>;

	template <typename T>
	using ostream_expression = decltype(
	    std::declval<std::ostringstream &>() << std::declval<T>().toString());

	template <typename T>
	constexpr auto has_ostream_to_string
	    = is_detected_v<std_to_string_expression, T>;

	template <typename T>
	inline std::string toString(T &&value) {
		if constexpr (has_to_string<T>)
			return to_string(std::forward<T>(value));
		else if constexpr (has_std_to_string<T>)
			return std::to_string(std::forward<T>(value));
		else if constexpr (has_member_to_string<T>)
			return value.toString();
		else if constexpr (has_ostream_to_string<T>) {
			auto oss = std::ostringstream {};
			oss << value;
			return std::move(oss).str();
		} else
			return "[Failed to find a function converter]";
	}

	namespace _private {
		////////////////////////////////////////
		////////////////////////////////////////
		template <typename T, typename... Args>
		std::string populate(const std::string &string, const size_t index,
		    T &&value, Args &&... args) {
			std::regex  regex("%\\{" + std::to_string(index) + "\\}");
			std::string value_string
			    = storm::core::toString<const T>(std::forward<T>(value));

			if constexpr (!sizeof...(args))
				return std::regex_replace(string, regex, value_string);
			else
				return populate(std::regex_replace(string, regex, value_string),
				    index + 1, std::forward<Args>(args)...);
		}
	}

	////////////////////////////////////////
	////////////////////////////////////////
	template <typename... Args>
	std::string format(const std::string &format, Args &&... args) {
		return _private::populate<Args...>(
		    format, 1, std::forward<Args>(args)...);
	}
}

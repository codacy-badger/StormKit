#pragma once
/**
 * @file Singleton.hpp
 * @brief A Singleton
 * @author Arthapz
 * @version 0.1
 **/

#include <mutex>
#include <functional>
#include <memory>
#include <utility>

#include <storm/core/NonMovable.hpp>
#include <storm/core/NonCopyable.hpp>
#include <storm/core/Memory.hpp>
#include <storm/log/LogOutput.hpp>

namespace storm::core {
	/**
	 * @class Singleton
	 * @ingroup Core
	 * @brief A Singleton class
	 *
	 * The class allow a unique class instance while runtime
	 */
	template <class T>
    class STORM_PUBLIC Singleton : public NonMovable, public NonCopyable {
		public:
			/**
			 * @brief Get the singleton
			 *
			 * Method wich if the instance don't exist, make an instance, and return it
			 *
			 * @param args : argument of the Singleton (variadic arguments)
			 * @return the instance of the singleton
			 */
			template <typename... Args>
			static T& instance(Args&&... args) {
				auto lambdas = [](Args&&... args) mutable {
					m_instance = std::make_unique<T>(std::forward<Args>(args)...);
				};

				std::call_once(onceFlag(), lambdas, std::forward<Args>(args)...);

				return *m_instance;
			}
		protected:
			explicit Singleton<T>() = default;
			~Singleton<T>() = default;

		private:
			static std::once_flag& onceFlag() {
				static auto once_flag = std::once_flag{};
				return once_flag;
			}
			static inline std::unique_ptr<T> m_instance = nullptr;
	};
}

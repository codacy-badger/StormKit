#pragma once

#include <vector>

#include <storm/core/Singleton.hpp>

#include <storm/reflection/Function.hpp>
#include <storm/reflection/Exceptions.hpp>

namespace storm::reflection {
	class ReflectableManager final : public storm::core::Singleton<ReflectableManager> {
		public:
			explicit ReflectableManager() = default;

			void registerReflectableFunction(std::unique_ptr<FunctionBase> &&reflectable) {
				m_functions.emplace_back(std::move(reflectable));
			}

			template <typename ReturnType, typename... Params>
			ReturnType reflect(const std::string &function_name, Params&&... params) {
				for(const auto &reflectable : m_functions) {
					if(reflectable->name() == function_name) {
						auto reflected = dynamic_cast<Function<ReturnType(Params...)>*>(reflectable.get());

						if(!reflected)
							throw IncorrectParametersSignatureException(function_name);

						return reflected->operator()(std::forward<Params>(params)...);
					}
				}

				throw NotRegisteredException(function_name);
			}

		private:
			std::vector<std::unique_ptr<FunctionBase>> m_functions;
	};
}


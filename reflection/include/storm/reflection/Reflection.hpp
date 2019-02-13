#pragma once

#include <storm/reflection/ReflectableManager.hpp>

namespace storm::reflection {
	template <typename ReturnType, typename... Params>
	struct Reflection;

	template <typename ReturnType, typename... Params>
	struct Reflection<ReturnType(Params...)> {
		static ReturnType invoke(const std::string &name, Params &&... params) {
			return ReflectableManager::instance()
			    .reflect<ReturnType, Params...>(
			        name, std::forward<Params>(params)...);
		}

		static ReturnType invoke(const std::string &class_name,
		    const std::string &                     name, Params &&... params) {
			return ReturnType(); // ReflectableManager::instance().reflect<ReturnType,
			                     // Params...>(name,
			                     // std::forward<Params>(params)...);
		}

		template <typename Object>
		static ReturnType invoke(Object &object, const std::string &class_name,
		    const std::string &name, Params &&... params) {
			return ReturnType(); ////ReflectableManager::instance().reflect<ReturnType,
			                     ///Params...>(name,
			                     ///std::forward<Params>(params)...);
		}
	};
}

#define REGISTER_FUNCTION(func)                                                \
	storm::reflection::ReflectableManager::instance()                          \
	    .registerReflectableFunction(                                          \
	        std::make_unique<storm::reflection::Function<decltype(func)>>(     \
	            #func, func));

#define REGISTER_FUNCTION_CN(func_name, func)                                  \
	storm::reflection::ReflectableManager::instance()                          \
	    .registerReflectableFunction(                                          \
	        std::make_unique<storm::reflection::Function<decltype(func)>>(     \
	            func_name, func));

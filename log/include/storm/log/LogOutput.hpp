#pragma once

/**
 * @file LogOutput.hpp
 * @brief Log Output base class
 * @author Arthapz
 * @version 0.1
 **/

#include <iostream>
#include <string>
#include <memory>
#include <regex>
#include <memory>

#include <storm/core/Strings.hpp>
#include <storm/core/NamedType.hpp>
#include <storm/core/Platform.hpp>

namespace storm {
	namespace log {
		/**
		 * @class Log
		 * @brief The abstract Logger
		 *
		 * The class allow implement your personnal logger
		 */

		using Module = core::NamedType<const char *, struct ModuleParameter>;
		class STORM_PUBLIC LogOutput
		{
			public:
				/**
				 * @enum Severity
				 * @brief The Severity enum
				 *
				 * The enum handle the severity of your message
				 */
				enum class Severity {
					Info, Warning, Error, FatalError, Debug, Nop
				};

				/**
				 * @brief Constructor
				 *
				 * Default Constructor of LogOutput
				 */
				explicit LogOutput() = default;

				/**
				 * @brief Destructor
				 *
				 * Destructor of LogOutput
				 */
				virtual ~LogOutput() = default;

				/**
				 * @brief send a message to the current logger
				 *
				 * Method which send a message to the current logger
				 *
				 * @param severity : the severity of the message (Severity)
				 * @param text : message (char *)
				 */
				void log(Severity severity,const char *text) {
					getBase(severity, nullptr);

					auto _text = std::string{text};

					write(_text.c_str());
				}

				/**
				 * @brief send a message to the current logger
				 *
				 * Method which send a message to the current logger
				 *
				 * @param severity : the severity of the message (Severity)
				 * @param module : the module of the message (Module)
				 * @param text : message (char *)
				 */
				void log(Severity severity, Module module, const char *text) {
					getBase(severity,  module.get());

					auto _text = std::string{text};

					write(_text.c_str());
				}

				/**
				 * @brief send a message to the current logger with arguments
				 *
				 * Method which send a message to the current logger with arguments
				 *
				 * @param severity : the severity of the message (Severity)
				 * @param text : message (char *)
				 * @param args : arguments (Variadic Args)
				 */
				template <typename ...Args>
				void log(Severity severity, const char *_format, Args&&... args) {
					getBase(severity, nullptr);

					auto _text = core::format(std::string(_format), std::forward<Args>(args)...);

					write(_text.c_str());
				}

				/**
				 * @brief send a message to the current logger with arguments
				 *
				 * Method which send a message to the current logger with arguments
				 *
				 * @param severity : the severity of the message (Severity)
				 * @param text : message (char *)
				 * @param module : the module of the message (Module)
				 * @param args : arguments (Variadic Args)
				 */
				template <typename ...Args>
				void log(Severity severity, Module module, const char *_format, Args&&... args) {
					getBase(severity,  module.get());

					auto _text = core::format(std::string(_format), std::forward<Args>(args)...);

					write(_text.c_str());
				}

				/**
				 * @brief set the current logger
				 *
				 * Method which set the current logger
				 *
				 * @param args : parameters for the logger (Variadic Args)
				 */
				template <class T,typename ...Args>
				static void installLogger(Args&&... args) {
					static_assert(std::is_base_of<LogOutput,T>::value,
									"T must inherit Log");
					currentLogger = std::make_unique<T, Args...>(std::forward<Args>(args)...);
				}

				/**
				 * @brief get the current logger
				 *
				 * Method which get the current logger
				 *
				 * @param args : parameters for the logger (Variadic Args)
				 */
				static LogOutput &getCurrentLogger();

			protected:
				static const std::string &getCurrentSeverity() noexcept;
				static void getBase(const Severity &severity, const char* module) noexcept;

			private:
				virtual void write(const char *string) = 0;

				static std::unique_ptr<LogOutput> currentLogger;
		};
	}


	template <typename ...Args>
	inline void DLOG(Args&&... args) {
#ifdef DEBUG_LOGGING
		log::LogOutput::getCurrentLogger().log(log::LogOutput::Severity::Debug,std::forward<Args>(args)...);
#endif
	}

	template <typename ...Args>
	inline void ILOG(Args&&... args) {
		log::LogOutput::getCurrentLogger().log(log::LogOutput::Severity::Info,std::forward<Args>(args)...);
	}

	template <typename ...Args>
	inline void WLOG(Args&&... args) {
		log::LogOutput::getCurrentLogger().log(log::LogOutput::Severity::Warning,std::forward<Args>(args)...);
	}

	template <typename ...Args>
	inline void ELOG(Args&&... args) {
		log::LogOutput::getCurrentLogger().log(log::LogOutput::Severity::Error,std::forward<Args>(args)...);
	}

	template <typename ...Args>
	inline void FLOG(Args&&... args) {
		log::LogOutput::getCurrentLogger().log(log::LogOutput::Severity::FatalError,std::forward<Args>(args)...);
	}
	template <typename ...Args>
	inline void ALOG(Args&&... args) {
			log::LogOutput::getCurrentLogger().log(log::LogOutput::Severity::Nop,std::forward<Args>(args)...);
	}

	STORM_PUBLIC void InstallDefaultLogOutput();
}

constexpr storm::log::Module operator"" _module(const char *str, std::size_t sz) {
	return storm::log::Module{str};
}

#include <storm/log/ConsoleOutput.hpp>
#include <storm/log/LogOutput.hpp>

using namespace storm::log;

std::unique_ptr<LogOutput> LogOutput::currentLogger = nullptr;
std::string                currentSeverity          = "";

/////////////////////////////////////
/////////////////////////////////////
LogOutput &LogOutput::getCurrentLogger() { return *currentLogger; }

namespace storm {
	/////////////////////////////////////
	/////////////////////////////////////
	void InstallDefaultLogOutput() {
		storm::log::LogOutput::installLogger<storm::log::ConsoleOutput>();
	}
}

/////////////////////////////////////
/////////////////////////////////////
const std::string &LogOutput::getCurrentSeverity() noexcept {
	return currentSeverity;
}

/////////////////////////////////////
/////////////////////////////////////
void LogOutput::getBase(const Severity &severity, const char *module) noexcept {
	if (module) {
		switch (severity) {
		case Severity::Info:
			currentSeverity = core::format("[Information, %{1}] ", module);
			break;
		case Severity::Warning:
			currentSeverity = core::format("[Warning, %{1}] ", module);
			break;
		case Severity::Error:
			currentSeverity = core::format("[Error, %{1}] ", module);
			break;
		case Severity::FatalError:
			currentSeverity = core::format("[FatalError, %{1}] ", module);
			break;
		case Severity::Debug:
			currentSeverity = core::format("[Debug, %{1}] ", module);
			break;
		default:
			currentSeverity = "";
			break;
		}
	} else {
		switch (severity) {
		case Severity::Info:
			currentSeverity = "[Information]";
			break;
		case Severity::Warning:
			currentSeverity = "[Warning]";
			break;
		case Severity::Error:
			currentSeverity = "[Error]";
			break;
		case Severity::FatalError:
			currentSeverity = "[FatalError]";
			break;
		case Severity::Debug:
			currentSeverity = "[Debug]";
			break;
		default:
			currentSeverity = "";
			break;
		}
	}
}

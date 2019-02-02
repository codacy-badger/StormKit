#pragma once

/**
 * @file FileOutput.hpp
 * @brief File Output class
 * @author Arthapz
 * @version 0.1
 **/

#include <storm/core/Filesystem.hpp>

#include <storm/log/LogOutput.hpp>

#include <fstream>

namespace storm::log {
	/**
	 * @class FileLog
	 * @ingroup Log
	 * @brief File Logger
	 *
	 * The class allow logging to a file
	 */
	class STORM_PUBLIC FileOutput final : public LogOutput {
		public:
			/**
			 * @brief Constructor
			 *
			 * Default Constructor of FileOutput
			 */
			explicit FileOutput(const _std::filesystem::path &filepath);

			/**
			 * @brief Destructor
			 *
			 * Destructor of FileOutput
			 */
			~FileOutput() override = default;

		private:
			void write(const char *string) override;

			std::ofstream m_file;
	};
}

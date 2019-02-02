#include <storm/log/FileOutput.hpp>
#include <storm/core/Assert.hpp>

using namespace storm::log;

/////////////////////////////////////
/////////////////////////////////////
FileOutput::FileOutput(const _std::filesystem::path &filepath) {
	m_file.open(filepath.c_str());
	ASSERT(m_file.is_open(), "Failed to open log file");
}

/////////////////////////////////////
/////////////////////////////////////
void FileOutput::write(const char *string) {
	if(!m_file)
		return;

    m_file << getCurrentSeverity() << " " << string << std::endl;
}


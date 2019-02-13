/*
 * @file b.hpp
 * @brief
 * @author Arthapz
 * @date
 */

#include "SharedLibraryImpl.hpp"

#include <stdexcept>

#ifdef STORM_POSIX

extern "C" {
#include <dlfcn.h>
}

using namespace storm::posix;

////////////////////////////////////////
////////////////////////////////////////
SharedLibraryImpl::SharedLibraryImpl(const std::string &filename)
    : sharedlibrary::_private::SharedLibraryImpl(filename),
      m_library_handle(nullptr) {
	m_library_handle = dlopen(filename.c_str(), RTLD_LAZY);

	if (!m_library_handle) {
		auto system_reason = std::string(dlerror());
		throw std::runtime_error(u8"Failed to load Shared object (" + m_filename
		                         + u8") system reason (" + system_reason
		                         + u8")");
	}
}

////////////////////////////////////////
////////////////////////////////////////
SharedLibraryImpl::~SharedLibraryImpl() { dlclose(m_library_handle); }

////////////////////////////////////////
////////////////////////////////////////
void *SharedLibraryImpl::_getFunc(const std::string &name) const {
	void *func = dlsym(m_library_handle, name.c_str());

	if (!func) {
		auto systemReason = std::string(dlerror());
		throw std::runtime_error(
		    u8"Failed to load Shared function (" + name + u8") from ("
		    + m_filename + u8") system reason (" + systemReason + u8")");
	}

	return func;
}

#endif

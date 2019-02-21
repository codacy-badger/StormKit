/*
 * @file b.hpp
 * @brief
 * @author Arthapz
 * @date
 */

#include "SharedLibraryImpl.hpp"

#ifdef STORM_OS_WINDOWS

using namespace storm::win32;

////////////////////////////////////////
////////////////////////////////////////
SharedLibraryImpl::SharedLibraryImpl(const std::string &filename)
      : sharedlibrary::_private::SharedLibraryImpl(filename), m_library_handle(nullptr) {
    m_library_handle =
      LoadLibrary(m_filename.c_str()); // TODO handle failing to load the shared object
}

////////////////////////////////////////
////////////////////////////////////////
SharedLibraryImpl::~SharedLibraryImpl() {
    FreeLibrary(m_library_handle);
}

////////////////////////////////////////
////////////////////////////////////////
void *SharedLibraryImpl::_getFunc(const std::string &name) const {
    return reinterpret_cast<void *>(GetProcAddress(m_library_handle, name.c_str()));
}

#endif

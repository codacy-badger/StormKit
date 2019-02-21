#include <storm/sharedobject/SharedLibrary.hpp>

#if defined(STORM_OS_WINDOWS)
#    include "impl/Win32/SharedLibraryImpl.hpp"
using Impl = storm::win32::SharedLibraryImpl;
#elif defined(STORM_POSIX)
#    include "impl/Posix/SharedLibraryImpl.hpp"
using Impl = storm::posix::SharedLibraryImpl;
#endif

using namespace storm::sharedlibrary;

SharedLibrary::SharedLibrary(const std::string &filename) : m_impl(nullptr) {
    assert(!filename.empty());

    m_impl = std::make_unique<Impl>(filename);
}

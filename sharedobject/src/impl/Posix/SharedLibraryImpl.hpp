#pragma once
#include <storm/core/Platform.hpp>
#include <storm/sharedobject/SharedLibrary.hpp>

#ifdef STORM_POSIX

namespace storm::posix {
    class SharedLibraryImpl final : public sharedlibrary::_private::SharedLibraryImpl {
    public:
        explicit SharedLibraryImpl(const std::string &filename);
        virtual ~SharedLibraryImpl();

    protected:
        void *_getFunc(const std::string &name) const override;

    private:
        void *m_library_handle;
    };
} // namespace storm::posix

#endif

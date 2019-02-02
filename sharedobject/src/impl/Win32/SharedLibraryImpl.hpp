#pragma once
#include <storm/sharedobject/SharedLibrary.hpp>
#include <storm/core/Platform.hpp>

#ifdef STORM_OS_WINDOWS

extern "C" {
	#include <windows.h>
}

namespace storm::win32 {
	class SharedLibraryImpl final : public sharedlibrary::_private::SharedLibraryImpl {
		public:
			explicit SharedLibraryImpl(const std::string &filename);
			virtual ~SharedLibraryImpl();

		protected:
			void *_getFunc(const std::string &name) const override;

		private:
			HMODULE m_library_handle;
	};
}

#endif

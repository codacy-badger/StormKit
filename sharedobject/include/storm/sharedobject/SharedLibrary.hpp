#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/core/Strings.hpp>
#include <storm/core/Platform.hpp>

#include <cassert>
#include <memory>
#include <functional>
#include <string>

/**
 * @file SharedLibrary.hpp
 * @brief Shared Library tools
 * @author Arthapz
 * @version 0.1
 **/

#ifdef STORM_OS_WINDOWS
#define STDCALL __stdcall
#else
#define STDCALL
#endif

namespace storm::sharedlibrary {
	namespace _private {
		class SharedLibraryImpl {
			public:
				explicit SharedLibraryImpl(const std::string &filename)
					: m_filename(filename) { }

				virtual ~SharedLibraryImpl() = default;

				template <class Signature>
				std::function<Signature> getFunc(const std::string &name) {
					return std::function<Signature>(reinterpret_cast<STDCALL Signature*>(_getFunc(name)));
				}

				const std::string &getFilename() const {
					return m_filename;
				}
			protected:
				virtual void *_getFunc(const std::string &name) const = 0;

				std::string m_filename;
		};
	}

	/**
	 * @class SharedLibrary
	 * @ingroup Core
	 * @brief Shared Library representation class
	 *
	 * The class allow a simple access cross-os to a shared library
	 */
	class STORM_PUBLIC SharedLibrary : public storm::core::NonCopyable {
		public:
			/**
			 * @brief Constructor
			 *
			 * Constructor of SharedLibrary
			 *
			 * @param filename : the filename of the shared library (string) assert(the filename can't be empty)
			 */
			explicit SharedLibrary(const std::string &filename);

			/**
			 * @brief Destructor
			 *
			 * Destructor of SharedLibrary
			 */
			~SharedLibrary() = default;

			SharedLibrary(SharedLibrary &&) = default;
			SharedLibrary &operator=(SharedLibrary &&) = default;

			/**
			 * @brief Get a shared symbol
			 *
			 * Method wich load and return the shared symbol of the shared library currently loaded
			 *
			 * @param name : the name of the shared symbol (string) assert(the name can't be empty)
			 */
			template <class Signature>
			std::function<Signature> getFunc(const std::string &name) const {
				assert(!name.empty());

				return m_impl->getFunc<Signature>(name);
			}

		private:
			std::unique_ptr<_private::SharedLibraryImpl> m_impl;
	};
}

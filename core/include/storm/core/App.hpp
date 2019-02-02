// Copryright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

/// \file core
/// \author Arthapz
///

#pragma once

#include <storm/core/NonCopyable.hpp>
#include <storm/core/NonMovable.hpp>

/// \module core
namespace storm::core {
	/// An app.
	///
	/// This is facility template to structure the entry point of the application
	class App : public storm::core::NonCopyable, public storm::core::NonMovable {
		public:
			/// \effect Construct an App object
			explicit App() noexcept = default;

			/// \effect Destruct an App object
			virtual ~App() noexcept = default;
			 
			/// \effect Run the virtual method _run
			/// \throws Anything whose throw on the _run virtual method and not handled
			inline void run(int argc,char **argv) {
				_run(argc, argv);
			}

		protected:
		    /// \effect Run the application
		    /// \throws Anything whose throw in the _run implentation and not handled
			virtual void _run(int argc,char **argv) = 0;
	};
}

#pragma once

#include <iostream>
#include <exception>

#ifdef ASSERT
#undef ASSERT
#endif

#if defined(__clang__) || defined(__GNUC__)
#define FUNCTION __PRETTY_FUNCTION__
#else
#define FUNCTION __FUNCSIG__
#endif

#ifndef NDEBUG
#define ASSERT(condition, message) \
	do { \
		if (! (condition)) { \
			std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
					  << " line " << __LINE__ << "\n " << FUNCTION << ": " << message << std::endl; \
			std::terminate(); \
		} \
	} while (false)
#else
#define ASSERT(condition, message) do { } while (false)
#endif

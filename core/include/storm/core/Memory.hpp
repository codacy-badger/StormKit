#pragma once

#include <storm/core/Platform.hpp>
#include <storm/core/NamedType.hpp>

#include <memory>
#include <type_traits>

namespace storm::core {
	template <class C>
	constexpr auto size(const C& c) -> decltype(c.size()) {
		return c.size();
	}
	template <class T, std::size_t N>
	constexpr std::size_t size(const T (&array)[N]) noexcept {
		return N;
	}
	using Bit = bool;

	template <typename T>
	constexpr inline bool getBit(T &&v, std::size_t pos) noexcept {
		return v & (1 << pos);
	}

	template <typename T>
	constexpr inline std::remove_cv_t<std::remove_reference_t<T>> getBits(const T &v, std::size_t offset, std::size_t size) noexcept {
		std::remove_const_t<std::remove_reference_t<T>> b = 0;

		for(std::uint_fast32_t i = 0;i < size; ++i)
			b or_eq (getBit(v, offset + i) << i);

		return b;
	}

	template <typename T>
	constexpr inline void setBit(T &t, bool &&v, std::size_t pos) noexcept {
		if(v)
			t |= (1 << pos);
		else
			t &= t ^ (1 << pos);
	}

	template <typename T>
	constexpr inline void setBit(T &t, const bool &v, std::size_t pos) noexcept {
		if(v)
			t |= (1 << pos);
		else
			t &= t ^ (1 << pos);
	}

	template <typename T, typename V>
	constexpr inline void setBits(T &t, V &&v, std::size_t pos, std::size_t size) noexcept {
		for(std::uint_fast32_t i = 0;i < size; ++i)
			setBit(t, (v >> i) & 1, pos + i);
	}

    #define Private_Shared_Object(x) using SPtr  = std::shared_ptr<x>; \
                                 using ConstSPtr = std::shared_ptr<const x>; \
								 using WPtr     = std::weak_ptr<x>; \
								 using CWPtr    = std::weak_ptr<const x>;

	#define Private_Shared_Object_With_Maker(x) Private_Shared_Object(x) \
						 template <typename ...ARGS> \
                         static inline SPtr makeShared(ARGS&&... args) { \
							struct EnableMaker : public x { EnableMaker(ARGS&&... args) : x(std::forward<ARGS>(args)...) {} }; \
							return std::make_shared<EnableMaker>(std::forward<ARGS>(args)...); \
						 }

    #define Private_Unique_Object(x, y) using Ptr = std::unique_ptr<x, y>; \
                                 using ConstPtr   = std::unique_ptr<const x, y>;

	#define Private_Unique_Object_With_Maker(x, y) Private_Unique_Object(x, y) \
								template <typename ...ARGS> \
                                static inline Ptr makeUnique(ARGS&&... args) { \
									struct EnableMaker : public x { EnableMaker(ARGS&&... args) : x(std::forward<ARGS>(args)...) {} }; \
									return std::make_unique<EnableMaker>(std::forward<ARGS>(args)...); \
								}


	#define Ref_Object(x) using RefW  = std::reference_wrapper<x>; \
						 using CRefW = std::reference_wrapper<const x>;
	#define Shared_Object_Wm(x) Private_Shared_Object(x)
	#define Shared_Object(x) Private_Shared_Object_With_Maker(x)
	#define Unique_Object(x) Private_Unique_Object_With_Maker(x, std::default_delete<x>)
	#define Unique_Object_Wm(x) Private_Unique_Object(x, std::default_delete<x>)
	#define Unique_Object_Custom_Deleter(x, y) Private_Unique_Object_With_Maker(x, y)
	#define Unique_Object_Custom_Deleter_Wm(x, y) Private_Unique_Objecter(x, y)
	#define SUR_Object(x) Shared_Object(x) \
						  Unique_Object(x) \
						  Ref_Object(x)
    #define SUR_Object_wm(x) Shared_Object_Wm(x) \
                             Unique_Object_Wm(x) \
                             Ref_Object(x)

	#define PADDING(x) std::byte private____padding[x];

	#if defined(STORM_COMPILER_MSVC)
		#define BEGIN_PACKED_STRUCT_A ___pragma(pack(push, 1))
		#define BEGIN_PACKED_STRUCT(x) BEGIN_PACKED_STRUCT_A struct x
		#define END_PACKET_STRUCT ___pragma(pack(pop))
    #elif defined(STORM_COMPILER_CLANG) || defined(STORM_COMPILER_GCC)
		#define BEGIN_PACKED_STRUCT_A __attribute__((__packed__))
		#define BEGIN_PACKED_STRUCT(x) struct BEGIN_PACKED_STRUCT_A x
		#define END_PACKED_STRUCT
	#else
		#warning Packed struct is not avalaible
		#define BEGIN_PACKED_STRUCT_A
		#define BEGIN_PACKED_STRUCT(x) BEGIN_PACKED_STRUCT_A struct x
		#define END_PACKED_STRUCT
	#endif

    #if defined(STORM_COMPILER_GCC) || defined(STORM_COMPILER_CLANG)
        #define SHARED_MEMORY_CUSTOM_LOCATION(var, value, location) var __attribute__((section(location), shared)) = value
    #elif defined(STORM_COMPILER_MSVC)
    #define SHARED_MEMORY_CUSTOM_LOCATION(var, value, location)  \
        ___pragma comment(linker, "/section:" location ",RWS")
        ___pragma data_seg(location) \
            var = value \
        ___pragma data_seg()
    #endif
    #define SHARED_MEMORY(var, value) SHARED_MEMORY_CUSTOM_LOCATION(var, value, ".shr");
}

#include "private/incbin.h"

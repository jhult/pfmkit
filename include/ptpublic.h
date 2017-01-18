//---------------------------------------------------------------------------
// Copyright 1997-2017 Joe Lowe
//
// Permission is granted to any person obtaining a copy of this Software,
// to deal in the Software without restriction, including the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and sell copies of
// the Software.
//
// The above copyright and permission notice must be left intact in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS WITHOUT WARRANTY.
//---------------------------------------------------------------------------
// filename:   ptpublic.h
// created:    2013.01.17
//
// Definitions for creating compiler, language (C/C++), and platform
// portable interfaces and definitions. Implementation code is not
// necessarily expected to use these definitions.
//---------------------------------------------------------------------------
#ifndef PTPUBLIC_H
#define PTPUBLIC_H

#ifndef PT_EXPAND
#define PT_EXPAND_(a) a
#define PT_EXPAND(a) PT_EXPAND_(a)
#endif
#ifndef PT_CAT
#define PT_CAT_(a,b) a##b
#define PT_CAT(a,b) PT_CAT_(a,b)
#endif
#ifndef PT_QUOTE
#define PT_QUOTE_(s) #s
#define PT_QUOTE(s) PT_QUOTE_(s)
#endif
#ifndef PT_LQUOTE
#define PT_LQUOTE(s) PT_CAT(L,PT_QUOTE_(s))
#endif

#if !defined(RC_INVOKED) && !defined(RC_PLIST)

#if (defined(INT_MAX) && LONG_MAX > INT_MAX) || defined(_WIN64) || defined(__x86_64__) || defined(__powerpc64__) || defined(__ppc64__) || defined(__mips64__) || defined(__arm64__) || defined(__ia64__)
 #define PT_SIZEOF_PTR 64
#else
 #define PT_SIZEOF_PTR 32
#endif
#if defined(LITTLEENDIAN) || defined(__LITTLE_ENDIAN__) || defined(WORDS_LITTLEENDIAN) || defined(_WIN32)
 #define PT_LITTLEENDIAN
#elif defined(BIGENDIAN) || defined(__BIG_ENDIAN__) || defined(WORDS_BIGENDIAN)
 #define PT_BIGENDIAN
#elif defined(__powerpc__) || defined(__ppc__) || defined(__powerpc64__) || defined(__ppc64__)
 #define PT_BIGENDIAN
#else
 #define PT_LITTLEENDIAN
#endif
#if PT_SIZEOF_PTR == 64 && !defined(_WIN32)
 #define PT_SIZEOF_LONG 64
#else
 #define PT_SIZEOF_LONG 32
#endif

#define PT_CHAR8 char
#define PT_INT32 int
#define PT_UINT32 unsigned
#ifdef INT64_C
 #define PT_BOOL8 uint8_t
 #define PT_INT8 int8_t
 #define PT_UINT8 uint8_t
 #define PT_INT16 int16_t
 #define PT_UINT16 uint16_t
 #define PT_INT64 int64_t
 #define PT_UINT64 uint64_t
#else
 #define PT_BOOL8 unsigned char
 #define PT_INT8 signed char
 #define PT_UINT8 unsigned char
 #define PT_INT16 short
 #define PT_UINT16 unsigned short
 #ifdef _MSC_VER
  #define PT_INT64 __int64
  #define PT_UINT64 unsigned __int64
  #define PT_INT64_MIN  (-9223372036854775807i64 - 1)
  #define PT_INT64_MAX  9223372036854775807i64
  #define PT_UINT64_MAX 0xffffffffffffffffui64
 #elif PT_SIZEOF_LONG == 64
  #define PT_INT64 long
  #define PT_UINT64 unsigned long
  #define PT_INT64_MIN  (-9223372036854775807L - 1)
  #define PT_INT64_MAX  9223372036854775807L
  #define PT_UINT64_MAX 0xffffffffffffffffL
 #else
  #define PT_INT64 long long
  #define PT_UINT64 unsigned long long
  #define PT_INT64_MIN  (-9223372036854775807LL - 1)
  #define PT_INT64_MAX  9223372036854775807LL
  #define PT_UINT64_MAX 0xffffffffffffffffLL
 #endif
#endif
#ifdef NULL
 #define PT_SIZE_T size_t
#elif PT_SIZEOF_PTR == 32
 #define PT_SIZE_T unsigned
#else
 #define PT_SIZE_T PT_UINT64
#endif

#ifdef _WIN32
 #define PT_CCALL __cdecl
 #define PT_STDCALL __stdcall
 #define PT_PUBLIC
 #define PT_PRIVATE
 #define PT_IMPORT __declspec(dllimport)
 #define PT_EXPORT __declspec(dllexport)
 #define PT_RESTRICT __restrict
 #ifdef _INC_WINDOWS
  #define PT_FD_T HANDLE
  #define PT_FD_INVALID INVALID_HANDLE_VALUE
 #else
  typedef void* pt_fd_t;
  #define PT_FD_T pt_fd_t
  #define PT_FD_INVALID ((void*)(ptrdiff_t)(-1))
 #endif
#else
 #define PT_CCALL
 #define PT_STDCALL
 #define PT_PUBLIC __attribute__((visibility("default")))
 #define PT_PRIVATE __attribute__((visibility("hidden")))
 #define PT_IMPORT
 #define PT_EXPORT __attribute__((visibility("default")))
 #define PT_RESTRICT __restrict__
 #define PT_FD_T int
 #define PT_FD_INVALID -1
#endif

#define PT_CAST(t,e) ((t)(e))
#define PT_TYPE_DECLARE(t) typedef struct t t
#define PT_TYPE_DEFINE2(t) struct t
#define PT_TYPE_DEFINE(t) typedef struct t t; struct t
#define PT_TYPEUNION_DECLARE(t) typedef union t t
#define PT_TYPEUNION_DEFINE2(t) union t
#define PT_TYPEUNION_DEFINE(t) typedef union t t; union t
#ifdef __cplusplus
 #define PT_SCAST(t,e) static_cast<t>(e)
 #define PT_CCAST(t,e) const_cast<t>(e)
 #define PT_RCAST(t,e) reinterpret_cast<t>(e)
 #define PT_EXTERNC extern "C"
 #define PT_EXTERNC_START extern "C" {
 #define PT_EXTERNC_END }
 #define PT_INLINE inline
 #define PT_INLINE2 inline
 #define PT_INLINE_BOOL PT_INLINE bool
 #define PT_STATIC_CONST(t,n,v) static const t n = v
 #define PT_INTERFACE_DECLARE(t) struct t
 #define PT_INTERFACE_DEFINE2 PT_TYPE_DEFINE2(INTERFACE_NAME)
 #define PT_INTERFACE_DEFINE PT_TYPE_DEFINE(INTERFACE_NAME)
 #define PT_INTERFACE_FUN0(r,m) virtual r PT_CCALL m(void) = 0
 #define PT_INTERFACE_FUN1(r,m,a) virtual r PT_CCALL m(a) = 0
 #define PT_INTERFACE_FUN2(r,m,a,b) virtual r PT_CCALL m(a,b) = 0
 #define PT_INTERFACE_FUN3(r,m,a,b,c) virtual r PT_CCALL m(a,b,c) = 0
 #define PT_INTERFACE_FUN4(r,m,a,b,c,d) virtual r PT_CCALL m(a,b,c,d) = 0
 #define PT_INTERFACE_FUN5(r,m,a,b,c,d,e) virtual r PT_CCALL m(a,b,c,d,e) = 0
 #define PT_INTERFACE_FUN6(r,m,a,b,c,d,e,f) virtual r PT_CCALL m(a,b,c,d,e,f) = 0
 #define PT_INTERFACE_FUN7(r,m,a,b,c,d,e,f,g) virtual r PT_CCALL m(a,b,c,d,e,f,g) = 0
 #define PT_INTERFACE_FUN8(r,m,a,b,c,d,e,f,g,h) virtual r PT_CCALL m(a,b,c,d,e,f,g,h) = 0
 #define PT_INTERFACE_FUN9(r,m,a,b,c,d,e,f,g,h,i) virtual r PT_CCALL m(a,b,c,d,e,f,g,h,i) = 0
 #define PT_INTERFACE_FU10(r,m,a,b,c,d,e,f,g,h,i,j) virtual r PT_CCALL m(a,b,c,d,e,f,g,h,i,j) = 0
 #define PT_INTERFACE_FU11(r,m,a,b,c,d,e,f,g,h,i,j,k) virtual r PT_CCALL m(a,b,c,d,e,f,g,h,i,j,k) = 0
 #define PT_INTERFACE_FU12(r,m,a,b,c,d,e,f,g,h,i,j,k,l) virtual r PT_CCALL m(a,b,c,d,e,f,g,h,i,j,k,l) = 0
 #define PT_INTERFACE_FU13(r,m,a,b,c,d,e,f,g,h,i,j,k,l,n) virtual r PT_CCALL m(a,b,c,d,e,f,g,h,i,j,k,l,n) = 0
 #define PT_VCAL0(o,m) ((o)->m())
 #if !defined(_MSC_VER) || _MSC_VER >= 1300
  #define PT_INTERFACE_FUNC(r,m,...) virtual r PT_CCALL m(__VA_ARGS__) = 0
  #define PT_VCALL(o,m,...) ((o)->m(__VA_ARGS__))
  #define PT_VCALLBASE(o,b,m,...) (PT_SCAST(b*,o)->m(__VA_ARGS__))
 #endif
#else
 #define PT_SCAST(t,e) ((t)(e))
 #define PT_CCAST(t,e) ((t)(e))
 #define PT_RCAST(t,e) ((t)(e))
 #define PT_EXTERNC
 #define PT_EXTERNC_START
 #define PT_EXTERNC_END
 #define PT_INLINE static __inline
 #define PT_INLINE2 static __inline
 #if !defined(_MSC_VER) || _MSC_VER >= 1800
  #define PT_INLINE_BOOL PT_INLINE _Bool
 #else
  #define PT_INLINE_BOOL PT_INLINE int
 #endif
 #define PT_STATIC_CONST(t,n,v) enum { n = v }
 #define PT_INTERFACE_DECLARE(t) typedef struct PT_CAT(t,_vtbl_t) PT_CAT(t,_vtbl_t); typedef PT_CAT(t,_vtbl_t)* t
 #define PT_INTERFACE_DEFINE2 struct PT_CAT(INTERFACE_NAME,_vtbl_t)
 #define PT_INTERFACE_DEFINE typedef struct PT_CAT(INTERFACE_NAME,_vtbl_t) PT_CAT(INTERFACE_NAME,_vtbl_t); typedef PT_CAT(INTERFACE_NAME,_vtbl_t)* INTERFACE_NAME; struct PT_CAT(INTERFACE_NAME,_vtbl_t)
 #define PT_INTERFACE_FUN0(r,m) r (PT_CCALL*m)(INTERFACE_NAME*)
 #define PT_INTERFACE_FUN1(r,m,a) r (PT_CCALL*m)(INTERFACE_NAME*,a)
 #define PT_INTERFACE_FUN2(r,m,a,b) r (PT_CCALL*m)(INTERFACE_NAME*,a,b)
 #define PT_INTERFACE_FUN3(r,m,a,b,c) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c)
 #define PT_INTERFACE_FUN4(r,m,a,b,c,d) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d)
 #define PT_INTERFACE_FUN5(r,m,a,b,c,d,e) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e)
 #define PT_INTERFACE_FUN6(r,m,a,b,c,d,e,f) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f)
 #define PT_INTERFACE_FUN7(r,m,a,b,c,d,e,f,g) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f,g)
 #define PT_INTERFACE_FUN8(r,m,a,b,c,d,e,f,g,h) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f,g,h)
 #define PT_INTERFACE_FUN9(r,m,a,b,c,d,e,f,g,h,i) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f,g,h,i)
 #define PT_INTERFACE_FU10(r,m,a,b,c,d,e,f,g,h,i,j) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f,g,h,i,j)
 #define PT_INTERFACE_FU11(r,m,a,b,c,d,e,f,g,h,i,j,k) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f,g,h,i,j,k)
 #define PT_INTERFACE_FU12(r,m,a,b,c,d,e,f,g,h,i,j,k,l) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f,g,h,i,j,k,l)
 #define PT_INTERFACE_FU13(r,m,a,b,c,d,e,f,g,h,i,j,k,l,n) r (PT_CCALL*m)(INTERFACE_NAME*,a,b,c,d,e,f,g,h,i,j,k,l,n)
 #define PT_VCAL0(o,m) ((*(o))->m(o))
 #if !defined(_MSC_VER) || _MSC_VER >= 1300
  #define PT_INTERFACE_FUNC(r,m,...) r (PT_CCALL*m)(PT_EXPAND(INTERFACE_NAME)*,__VA_ARGS__)
  #define PT_VCALL(o,m,...) ((*(o))->m(o,__VA_ARGS__))
  #define PT_VCALLBASE(o,b,m,...) ((*(o))->m(o,__VA_ARGS__))
 #endif
#endif

#define _PT_C_ASSERT_2(l) compile_time_assert_##l
#define _PT_C_ASSERT_1(e,l) typedef int _PT_C_ASSERT_2(l)[(e)?1:-1]
#define PT_C_ASSERT(e) _PT_C_ASSERT_1(e,__LINE__)
#define PT_C_ASSERT1(e) extern int compile_time_assert[(e)?1:-1]
#define PT_C_ASSERT2(e) do { PT_C_ASSERT(e); } while(0)
#ifndef PT_ASSERT
 #ifdef ASSERT
  #define PT_ASSERT(e) ASSERT(e)
 #else
  #define PT_ASSERT(e)
 #endif
#endif

PT_TYPE_DEFINE(pt_uuid_t) { PT_UINT32 data1; PT_UINT16 data2; PT_UINT16 data3; PT_UINT8 data4[8]; };
#define PT_UUID pt_uuid_t
PT_TYPE_DEFINE( pt_int16u_t) { PT_UINT8 d[2]; };
PT_TYPE_DEFINE( pt_int32u_t) { PT_UINT8 d[4]; };
PT_TYPE_DEFINE( pt_int64u_t) { PT_UINT8 d[8]; };
PT_TYPE_DEFINE(pt_uint16u_t) { PT_UINT8 d[2]; };
PT_TYPE_DEFINE(pt_uint32u_t) { PT_UINT8 d[4]; };
PT_TYPE_DEFINE(pt_uint64u_t) { PT_UINT8 d[8]; };
PT_TYPEUNION_DEFINE( pt_int16oe_t) {  PT_INT16 oe;  pt_int16u_t u; };
PT_TYPEUNION_DEFINE( pt_int32oe_t) {  PT_INT32 oe;  pt_int32u_t u; };
PT_TYPEUNION_DEFINE( pt_int64oe_t) {  PT_INT64 oe;  pt_int64u_t u; };
PT_TYPEUNION_DEFINE(pt_uint16oe_t) { PT_UINT16 oe; pt_uint16u_t u; };
PT_TYPEUNION_DEFINE(pt_uint32oe_t) { PT_UINT32 oe; pt_uint32u_t u; };
PT_TYPEUNION_DEFINE(pt_uint64oe_t) { PT_UINT64 oe; pt_uint64u_t u; };
PT_TYPE_DEFINE( pt_int16oeu_t) {  pt_int16u_t u; };
PT_TYPE_DEFINE( pt_int32oeu_t) {  pt_int32u_t u; };
PT_TYPE_DEFINE( pt_int64oeu_t) {  pt_int64u_t u; };
PT_TYPE_DEFINE(pt_uint16oeu_t) { pt_uint16u_t u; };
PT_TYPE_DEFINE(pt_uint32oeu_t) { pt_uint32u_t u; };
PT_TYPE_DEFINE(pt_uint64oeu_t) { pt_uint64u_t u; };
PT_TYPE_DEFINE(pt_uuidu_t  ) { pt_uint32u_t   data1; pt_uint16u_t   data2;   pt_uint16u_t data3; PT_UINT8 data4[8]; };
PT_TYPE_DEFINE(pt_uuidoe_t ) { pt_uint32oe_t  data1; pt_uint16oe_t  data2;  pt_uint16oe_t data3; PT_UINT8 data4[8]; };
PT_TYPE_DEFINE(pt_uuidoeu_t) { pt_uint32oeu_t data1; pt_uint16oeu_t data2; pt_uint16oeu_t data3; PT_UINT8 data4[8]; };
#ifdef PT_LITTLEENDIAN
 #define PT_INT16LE   PT_INT16
 #define PT_UINT16LE  PT_UINT16
 #define PT_INT32LE   PT_INT32
 #define PT_UINT32LE  PT_UINT32
 #define PT_INT64LE   PT_INT64
 #define PT_UINT64LE  PT_UINT64
 #define PT_INT16LEU  pt_int16u_t
 #define PT_UINT16LEU pt_uint16u_t
 #define PT_INT32LEU  pt_int32u_t
 #define PT_UINT32LEU pt_uint32u_t
 #define PT_INT64LEU  pt_int64u_t
 #define PT_UINT64LEU pt_uint64u_t
 #define PT_UUIDLE    pt_uuid_t
 #define PT_UUIDLEU   pt_uuidu_t
 #define PT_INT16BE   pt_int16oe_t
 #define PT_UINT16BE  pt_uint16oe_t
 #define PT_INT32BE   pt_int32oe_t
 #define PT_UINT32BE  pt_uint32oe_t
 #define PT_INT64BE   pt_int64oe_t
 #define PT_UINT64BE  pt_uint64oe_t
 #define PT_INT16BEU  pt_int16oeu_t
 #define PT_UINT16BEU pt_uint16oeu_t
 #define PT_INT32BEU  pt_int32oeu_t
 #define PT_UINT32BEU pt_uint32oeu_t
 #define PT_INT64BEU  pt_int64oeu_t
 #define PT_UINT64BEU pt_uint64oeu_t
 #define PT_UUIDBE    pt_uuidoe_t
 #define PT_UUIDBEU   pt_uuidoeu_t
#else
 #define PT_INT16BE   PT_INT16
 #define PT_UINT16BE  PT_UINT16
 #define PT_INT32BE   PT_INT32
 #define PT_UINT32BE  PT_UINT32
 #define PT_INT64BE   PT_INT64
 #define PT_UINT64BE  PT_UINT64
 #define PT_INT16BEU  pt_int16u_t
 #define PT_UINT16BEU pt_uint16u_t
 #define PT_INT32BEU  pt_int32u_t
 #define PT_UINT32BEU pt_uint32u_t
 #define PT_INT64BEU  pt_int64u_t
 #define PT_UINT64BEU pt_uint64u_t
 #define PT_UUIDBE    pt_uuid_t
 #define PT_UUIDBEU   pt_uuidu_t
 #define PT_INT16LE   pt_int16oe_t
 #define PT_UINT16LE  pt_uint16oe_t
 #define PT_INT32LE   pt_int32oe_t
 #define PT_UINT32LE  pt_uint32oe_t
 #define PT_INT64LE   pt_int64oe_t
 #define PT_UINT64LE  pt_uint64oe_t
 #define PT_INT16LEU  pt_int16oeu_t
 #define PT_UINT16LEU pt_uint16oeu_t
 #define PT_INT32LEU  pt_int32oeu_t
 #define PT_UINT32LEU pt_uint32oeu_t
 #define PT_INT64LEU  pt_int64oeu_t
 #define PT_UINT64LEU pt_uint64oeu_t
 #define PT_UUIDLE    pt_uuidoe_t
 #define PT_UUIDLEU   pt_uuidoeu_t
#endif
#ifdef _WIN32
 #define PT_EXESUFFIXA    ".exe"
 #define PT_SHAREDPREFIXA ""
 #define PT_SHAREDEXTA    "dll"
 #define PT_PLUGINEXTA    "dll"
#elif defined(__APPLE__)
 #define PT_EXESUFFIXA    ""
 #define PT_SHAREDPREFIXA "lib"
 #define PT_SHAREDEXTA    "dylib"
 #define PT_PLUGINEXTA    "bundle"
#else
 #define PT_EXESUFFIXA    ""
 #define PT_SHAREDPREFIXA "lib"
 #define PT_SHAREDEXTA    "so"
 #define PT_PLUGINEXTA    "so"
#endif
#define PT_EXESUFFIXW    PT_CAT(L,PT_EXESUFFIXA)
#define PT_SHAREDPREFIXW PT_CAT(L,PT_SHAREDPREFIXA)
#define PT_SHAREDEXTW    PT_CAT(L,PT_SHAREDEXTA)
#define PT_PLUGINEXTW    PT_CAT(L,PT_PLUGINEXTA)

#define PT_SZ_T char*
#define PT_SZC_T const char*
#define PT_WSZ_T wchar_t*
#define PT_WSZC_T const wchar_t*

#endif

#endif

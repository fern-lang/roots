/*
  MIT License

  Copyright (c) 2023 Jules Nieves

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef _ROOTS_DEFINES_H_
#define _ROOTS_DEFINES_H_

#include <stdint.h>
#include <stddef.h>

#if defined(_WIN32) || defined(_WIN64)
#define ROOTS_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#define ROOTS_PLATFORM_APPLE
#elif defined(__linux__)
#define ROOTS_PLATFORM_LINUX
#elif defined(__unix__)
#define ROOTS_PLATFORM_UNIX
#else
#error "Unknown platform"
#endif

#ifndef __funcname__
#ifdef ROOTS_PLATFORM_WINDOWS
#define __funcname__ __FUNCTION__
#else
#define __funcname__ __func__
#endif
#endif

#ifndef __filename__
#ifdef ROOTS_PLATFORM_WINDOWS
#define __filename__ __FILE__
#else
#define __filename__ __FILE__
#endif
#endif

#if defined(__clang__)
#define ROOTS_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define ROOTS_COMPILER_GCC
#elif defined(_MSC_VER)
#define ROOTS_COMPILER_MSVC
#else
#error "Unknown compiler"
#endif

#if defined(ROOTS_COMPILER_CLANG) || defined(ROOTS_COMPILER_GCC)
#define ROOTS_DEPRECATED __attribute__((deprecated))
#elif defined(ROOTS_COMPILER_MSVC)
#define ROOTS_DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: ROOTS_DEPRECATED not supported for this compiler")
#define ROOTS_DEPRECATED
#endif

#if defined(ROOTS_COMPILER_CLANG) || defined(ROOTS_COMPILER_GCC)
#define ROOTS_NORETURN __attribute__((noreturn))
#elif defined(ROOTS_COMPILER_MSVC)
#define ROOTS_NORETURN __declspec(noreturn)
#else
#pragma message("WARNING: ROOTS_NORETURN not supported for this compiler")
#define ROOTS_NORETURN
#endif

#if __cplusplus
#define ROOTSC extern "C" {
#define ROOTSC_END }
#else
#define ROOTSC
#define ROOTSC_END
#endif

#if defined(ROOTS_COMPILER_CLANG) || defined(ROOTS_COMPILER_GCC)
#if __has_builtin(__builtin_unreachable)
#define ROOTS_UNREACHABLE __builtin_unreachable()
#else
#define ROOTS_UNREACHABLE for (;;) // technically UB, but works
#endif
#elif defined(ROOTS_COMPILER_MSVC)
#define ROOTS_UNREACHABLE __assume(0)
#else
#pragma message("WARNING: ROOTS_UNREACHABLE not supported for this compiler")
#define ROOTS_UNREACHABLE
#endif

#if defined(ROOTS_COMPILER_CLANG) || defined(ROOTS_COMPILER_GCC)
#define ROOTS_ASSERT(x) ((x) ? (void)0 : __assert_fail(#x, __filename__, __LINE__, __funcname__))
#elif defined(ROOTS_COMPILER_MSVC)
#define ROOTS_ASSERT(x) ((x) ? (void)0 : __assert(#x, __filename__, __LINE__))
#else
#pragma message("WARNING: ROOTS_ASSERT not supported for this compiler")
#define ROOTS_ASSERT(x) ((void)0)
#endif

#if defined(__builtin_bswap16)
#define ROOTS_BSWAP16 __builtin_bswap16
#elif defined(ROOTS_COMPILER_MSVC)
#define ROOTS_BSWAP16 _byteswap_ushort
#else
#define ROOTS_BSWAP16(x) (((x) >> 8) | ((x) << 8))
#endif

#if defined(__builtin_bswap32)
#define ROOTS_BSWAP32 __builtin_bswap32
#elif defined(ROOTS_COMPILER_MSVC)
#define ROOTS_BSWAP32 _byteswap_ulong
#else
#define ROOTS_BSWAP32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
#endif

#if defined(__builtin_bswap64)
#define ROOTS_BSWAP64 __builtin_bswap64
#elif defined(ROOTS_COMPILER_MSVC)
#define ROOTS_BSWAP64 _byteswap_uint64
#else
#define ROOTS_BSWAP64(x) (((x) >> 56) | (((x) & 0x00FF000000000000) >> 40) | (((x) & 0x0000FF0000000000) >> 24) | (((x) & 0x000000FF00000000) >> 8) | (((x) & 0x00000000FF000000) << 8) | (((x) & 0x0000000000FF0000) << 24) | (((x) & 0x000000000000FF00) << 40) | ((x) << 56))
#endif

#define ROOTS_BSWAP32F(x) ROOTS_BSWAP32(*(uint32_t*)&(x))
#define ROOTS_BSWAP64F(x) ROOTS_BSWAP64(*(uint64_t*)&(x))

#define ROOTS_LITTLE_ENDIAN 0x41424344UL
#define ROOTS_BIG_ENDIAN 0x44434241UL

#if defined(ROOTS_PLATFORM_WINDOWS)
#define ROOTS_BYTE_ORDER ROOTS_LITTLE_ENDIAN
#else
#include <sys/types.h>
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ROOTS_BYTE_ORDER ROOTS_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define ROOTS_BYTE_ORDER ROOTS_BIG_ENDIAN
#else
#error "Unknown endianness"
#endif
#endif

#define kRootsFilesystemMaxPathLength 4096

// Type aliases for common types
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
using usize = size_t;
using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;
using isize = ptrdiff_t;
using f32 = float;
using f64 = double;

#endif

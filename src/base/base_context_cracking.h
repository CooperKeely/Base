#ifndef BASE_CONTEXT_CRACKING_H
#define BASE_CONTEXT_CRACKING_H

//////////////////////////////////////
/// cjk: Context Cracking

// this is only used for better signalhandeling
#define _POSIX_C_SOURCE 200809L

#if defined(__STDC_VERSION__)
# if __STDC_VERSION__ < 199901L
#  error "base.h requires a C99 compatible compiler for higher"
# endif
#else
# error "base.h requires a C99 compatible compiler for higher"
#endif

#if defined(__clang__)

#define COMPILER_CLANG 1

#if defined(_WIN32)
# define OS_WINDOWS 1
#elif defined(__gnu_linux__) || defined(__linux__)
# define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
# define OS_MAC 1
#else
# error "This compiler/OS combo is not supported"
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64) || defined(__x86_64__)
# define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
# define ARCH_X86 1
#elif defined(__aarch64__)
# define ARCH_ARM64
#elif defined(__arm__)
# define ARCH_ARM32
#else
# error "Architecture not supported"
#endif

#elif defined(_MSC_VER)

#define COMPILER_MSVC 1

#if defined(_WIN32)
# define OS_WINDOWS 1
#else
# error "This compiler/OS combo is not supported"
#endif

#if defined(_M_AMD64)
# define ARCH_X64 1
#elif defined(_M_IX86)
# define ARCH_X86 1
#elif defined(_M_ARM64)
# define ARCH_ARM64 1
#elif defined(_M_ARM)
# define ARCH_ARM32 1
#else
# error "Architecture not supported"
#endif

#elif defined(__GNUC__) || defined(__GNUG__)

#define COMPILER_GCC 1

#if defined(__gnu_linux__) || defined(__linux__)
# define OS_LINUX 1
#else
# error "This compiler/OS combo is not supported"
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64) || defined(__x86_64__)
# define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
# define ARCH_X86 1
#elif defined(__aarch64__)
# define ARCH_ARM64 1
#elif defined(__arm__)
# define ARCH_ARM32 1
#else
# error "Architecture not supported"
#endif

#else
# error "Compiler not supported"
#endif

// clang compatability
#ifndef COMPILER_CLANG
# ifndef __has_feature
#  define __has_feature(x) 0
# endif
# ifndef __has_attribute
#  define __has_attribute(x) 0
# endif
# ifndef __has_builtin
#  define __has_builtin(x) 0
# endif
#endif

#if defined(__cplusplus)
# define LANG_CPP
#else
# define LANG_C
#endif

#if !defined(BUILD_DEBUG)
# define BUILD_DEBUG 1
#endif

#if defined(ARCH_X64)
# define ARCH_64BIT 1
#elif defined(ARCH_x86)
# define ARCH_32BIT 1
#endif

#if ARCH_ARM32 || ARCH_ARM64 || ARCH_X64 || ARCH_X86
# define ARCH_LITTLE_ENDIAN 1
#else
# error "Endianness of this architecture is not understood by context cracker"
#endif

// Zero all undefined options
#if !defined(ARCH_32BIT)
# define ARCH_32BIT 0
#endif
#if !defined(ARCH_64BIT)
# define ARCH_64BIT 0
#endif
#if !defined(ARCH_X64)
# define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
# define ARCH_X86 0
#endif
#if !defined(ARCH_ARM64)
# define ARCH_ARM64 0
#endif
#if !defined(ARCH_ARM32)
# define ARCH_ARM32 0
#endif
#if !defined(COMPILER_MSVC)
# define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
# define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
# define COMPILER_CLANG 0
#endif
#if !defined(OS_WINDOWS)
# define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
# define OS_LINUX 0
#endif
#if !defined(OS_MAC)
# define OS_MAC 0
#endif

// Throw error for unsupported platforms
#if OS_WINDOWS || OS_MAC
# error "OS not supported yet"
#elif COMPILER_MSVC
# error "Compiler not supported yet"
#elif !defined(ARCH_X64)
# error "Architecture not supported yet"
#endif

#endif //BASE_CONTEXT_CRACKING_H

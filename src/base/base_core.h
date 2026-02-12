#ifndef BASE_CORE_H
#define BASE_CORE_H

///////////////////////////////////////
/// cjk: File Includes
#if OS_LINUX
# define _GNU_SOURCE
# include <dlfcn.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
#endif

///////////////////////////////////////
/// cjk: Codebase Keywords

#define global static
#define internal static
#define local_persist static

#if COMPILER_MSVC || (COMPILER_CLANG && OS_WINDOWS)
# pragma section(".rdata$", read)
# define read_only __declspec(allocate(".rdata$"))
#elif (COMPILER_CLANG && OS_LINUX)
# define read_only __attribute__((section(".rodata")))
#else
# define read_only
#endif

#if COMPILER_MSVC
# define thread_static __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
# define thread_static __thread
#else
# error "thread_static not defined for this compiler"
#endif

#if COMPILER_MSVC
# define force_inline __forceinline
#elif COMPILER_CLANG || COMPILER_GCC
# define force_inline __attribute__((always_inline))
#else
# error "force_inline not defined for this compiler"
#endif

#if COMPILER_MSVC
# define no_discard _Check_return_
#elif COMPILER_CLANG || COMPILER_GCC
# define no_discard __attribute__((warn_unused_result))
#else
# error "no_discard not defined for this compiler"
#endif

///////////////////////////////////////
/// cjk: Linkage Keyword Macros

#if LANG_CPP
# define C_LINKAGE_BEGIN extern "C" {
# define C_LINKAGE_END }
# define C_LINKAGE extern "C"
#else
# define C_LINKAGE_BEGIN
# define C_LINKAGE_END
# define C_LINKAGE
#endif

///////////////////////////////////////
/// cjk: Brach Predictor Helper

#if COMPILER_CLANG
# define Expect(expr, val) __builtin_expect((expr), (val))
#else
# define Expect(expr, val) (expr)
#endif

#define Likely(x) Expect((x), (1))
#define Unlikely(x) Expect((x), (0))

///////////////////////////////////////
/// cjk: Type -> Alignment

#if COMPILER_MSVC
# define AlignOf(T) __alignof(T)
#elif COMPILER_CLANG
# define AlignOf(T) __alignof(T)
#elif COMPILER_GCC
# define AlignOf(T) __alignof__(T)
#else
# error "AlignOf not defined for this compiler"
#endif

#if COMPILER_MSVC
# define AlignType(x) __declspec(align(x))
#elif COMPILER_CLANG || COMPILER_GCC
# define AlignType(x) __attribute__((aligned(x))
#else
# error "AlignType not defined for this compiler"
#endif

///////////////////////////////////////
/// cjk: Error Handeling Definitions

#if COMPILER_MSVC
# define Trap() __debugbreak()
#elif COMPILER_CLANG || COMPILER_GCC
# define Trap() __builtin_trap()
#else
# error "Unknow trap for this compiler"
#endif

#if COMPILER_MSVC
# define MarkUnreachable() __assume(0)
#elif COMPILER_CLANG || COMPILER_GCC
# define MarkUnreachable() __builtin_unreachable()
#else
# error "Unknown unreachable for this compiler"
#endif

#if BUILD_DEBUG
# define Assert(condition) do {	if (Unlikely(!(condition))) {Trap();}} while (0)
# define InvalidPath do { if ((!"Invalid Code Path")) {Trap(); MarkUnreachable();}} while (0)
# define NotImplemented Assert(!"Not Implemented")

# ifdef __has_builtin
#  if __has_builtin(__builtin_types_compatible_p)
#   define IsType(x, T) __builtin_types_compatible_p(__typeof__(x), T)
#  endif
# else
#  define IsType(x, T) (void)0
#endif
#else
# define Assert(condition) (void)0
# define InvalidPath (void)0
# define NotImplemented (void)0
# define IsType(x, T) 1
#endif

#define StaticAssert(c, label) global U8 Glue(label, __LINE__)[(c) ? 1 : -1]

///////////////////////////////////////
/// cjk: Asan Definitions

#if COMPILER_MSVC
# if defined(__SANITIZE_ADDRESS__)
#  define ASAN_ENABLED 1
#  define NO_ASAN __declspec(no_sanitize_address)
# else
#  define ASAN_ENABLED 0
#  define NO_ASAN
# endif
#elif COMPILER_CLANG
# if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#  define ASAN_ENABLED 1
# else
#  define ASAN_ENABLED 0
# endif
# define NO_ASAN __attribute__((no_sanitize("address")))
#else
# define ASAN_ENABLED 0
# define NO_ASAN
#endif

#if ASAN_ENABLED
C_LINKAGE void __asan_poison_memory_region(void const volatile *addr,size_t size);
C_LINKAGE void __asan_unpoison_memory_region(void const volatile *addr,size_t size);
#define AsanPoisonMemoryRegion(addr, size) __asan_poison_memory_region((addr), (size))
#define AsanUnpoisonMemoryRegion(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
#define AsanPoisonMemoryRegion(addr, size) ((void)(addr), (void)(size))
#define AsanUnpoisonMemoryRegion(addr, size) ((void)(addr), (void)(size))
#endif

///////////////////////////////////////
/// cjk: Misc Macro Definitions 

#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Glue_(A, B) A##B
#define Glue(A, B) Glue_(A, B)

#define ArrayCount(a) (sizeof(a) / sizeof(a[0]))

#define AlignPow2(x, b) (((x) + (b) - 1)&(~((b) - 1))
#define AlignDownPow2(x, b) ((x)&(~((b) - 1))
#define IsPow2(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)

///////////////////////////////////////
/// cjk: Loop Helper Definitions

#define DeferLoop(begin, end) for (int _i_ = ((begin), 0); _i_ != 1; _i_ += 1, (end))

// For loop helpers
#define EachInRange(idx, range)	(U64 idx = (range).min; idx < (range).max; idx++)
#define EachIndex(idx, count) (U64 idx = 0; idx < (count); idx++)
#define EachNode(idx, T, first) (T *idx = first; idx != NULL; idx = idx->next)

///////////////////////////////////////
/// cjk: Units Definitions

#define KB(n) (((U64)(n)) << 10)
#define MB(n) (((U64)(n)) << 20)
#define GB(n) (((U64)(n)) << 30)

#define Thousand(n) ((n) * 1000)
#define Million(n) ((n) * Thousand(1000))
#define Billion(n) ((n) * Million(1000))

///////////////////////////////////////
/// cjk: Memory Macro Definitions

#define MemoryCopy(dest, src, num_bytes) memmove((dest), (src), (num_bytes))
#define MemorySet(dest, byte, num_bytes) memset((dest), (byte), (num_bytes))
#define MemoryCmp(a, b, num_bytes) memcmp((a), (b), (num_bytes))
#define MemoryStrlen(ptr) strlen(ptr)

#define MemoryCopyStruct(dest, src) MemoryCopy((dest), (src), sizeof(*(dest)))
#define MemoryCopyArray(dest, src) MemoryCopy((dest), (src), sizeof(dest))
#define MemoryCopyStr8(dest, src) MemoryCopy((dest).str, (src).str, (src).size)

#define MemoryZero(dest, num_bytes) memset((dest), 0, (num_bytes))
#define MemoryZeroStruct(src) MemoryZero((src), sizeof(*(s)))
#define MemoryZeroArray(src) MemoryZero((src), sizeof(src))

#define MemoryMatch(a, b, num_bytes) (MemoryCmp((a), (b), (num_bytes)) == 0)
#define MemoryMatchStruct(a, b) MemoryMatch((a), (b), sizeof(*(a)))
#define MemoryMatchArray(a, b) MemoryMatch((a), (b), sizeof((a)))

///////////////////////////////////////
/// cjk: Math Helper Definitions

#define BitsFromBytes(x) (8 * (x)) 
#define Min(A, B) (((A) < (B)) ? (A) : (B))
#define Max(A, B) (((A) > (B)) ? (A) : (B))

#define Swap(A, B) do{					\
		__typeof__(A) __temp__ = (A);		\
		(A)= (B); 				\
		(B) = __temp__;				\
	}while(0)

#define ClampTop(A, B) Min(A, B)
#define ClampBot(A, B) Max(A, B)

#define false 0
#define true 1

///////////////////////////////////////
/// cjk: Basic Number Definitions 

#define U64Lit(x) Glue(x, UL)
#define S64Lit(x) Glue(x, L)
#define F64Lit(x) Glue(x, )

#define U32Lit(x) Glue(x, U)
#define S32Lit(x) Glue(x, )
#define F32Lit(x) Glue(x, F)

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef float F32;
typedef double F64;

typedef S8 B8;
typedef S16 B16;
typedef S32 B32;
typedef S64 B64;

typedef union {
  U8 u8[16];
  U16 u16[8];
  U32 u32[4];
  U64 u64[2];
} U128;

typedef union {
  U8 u8[32];
  U16 u16[16];
  U32 u32[8];
  U64 u64[4];
  U128 u128[2];
} U256;

typedef union {
  U8 u8[64];
  U16 u16[32];
  U32 u32[16];
  U64 u64[8];
  U128 u128[4];
  U256 u256[2];
} U512;

// Check literals work
StaticAssert(IsType(U32Lit(10), U32), U32Lit);
StaticAssert(IsType(S32Lit(1), S32), S32Lit);
StaticAssert(IsType(F32Lit(1.0), F32), F32Lit);
StaticAssert(IsType(U64Lit(10), U64), U64Lit);
StaticAssert(IsType(S64Lit(-1), S64), S64Lit);
StaticAssert(IsType(F64Lit(1.0), F64), F64Lit);


// int type conversions
U8 safe_cast_U8(U16 x);
U16 safe_cast_U16(U32 x);
U32 safe_cast_U32(U64 x);

S8 safe_cast_S8(S16 x);
S16 safe_cast_S16(S32 x);
S32 safe_cast_S32(S64 x);


// constants
global const U64 max_U64 = U64Lit(0xFFFFFFFFFFFFFFFF);
global const U64 max_S64 = S64Lit(0x7FFFFFFFFFFFFFFF);
global const U64 min_U64 = U64Lit(0x0000000000000000);
global const U64 min_S64 = S64Lit(0x8000000000000000);

global const U32 max_U32 = U32Lit(0xFFFFFFFF);
global const S32 max_S32 = S32Lit(0x7FFFFFFF);
global const U32 min_U32 = U32Lit(0x00000000);
global const S32 min_S32 = S32Lit(0x80000000);

global const U16 max_U16 = 0xFFFF;
global const S16 max_S16 = 0x7FFF;
global const U16 min_U16 = 0x0000;
global const S16 min_S16 = 0x8000;

global const U8 max_U8 = 0xFF;
global const S8 max_S8 = 0x7F;
global const U8 min_U8 = 0x00;
global const S8 min_S8 = 0x80;

global const F32 pi32 = F32Lit(3.1415926535897);

///////////////////////////////////////
/// cjk: Time Definitions

typedef enum {
	WeekDay_Sun,
	WeekDay_Mon,
	WeekDay_Tue,
	WeekDay_Wed,
	WeekDay_Thu,
	WeekDay_Fri,
	WeekDay_Sat,
	WeekDay_COUNT
} WeekDay;

typedef enum {
	Month_Jan,
	Month_Feb,
	Month_Mar,
	Month_Apr,
	Month_May,
	Month_Jun,
	Month_Jul,
	Month_Aug,
	Month_Sep,
	Month_Oct,
	Month_Nov,
	Month_Dec,
	Month_COUNT
} Month;

typedef struct {
	U16 micro_sec; // [0, 999]
	U16 mil_sec;	 // [0 , 999]
	U16 sec;	 // [0, 60]
	U16 min;	 // [0, 59]
	U16 hour;	 // [0 23]
	U16 day;	 // [0 30]
	union {
		WeekDay week_day;
		U32 week_day_num;
	};
	union {
		Month month;
		U32 month_num;
	};
	U32 year; // BC = 0, AD = 1
} DateTime;

typedef U64 DenseTime;

DenseTime densetime_from_datetime(DateTime time);
DateTime datetime_from_densetime(DenseTime time);
DateTime datetime_from_unixtime(U64 unix_time);


#endif // BASE_CORE_H

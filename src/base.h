#ifndef BASE_H
#define BASE_H

///////////////////////////////////////
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
#define MemoryCopyStr8(dest, src) MemoryCopy((dest), (src).str, (src).size))

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

#define ClampMax(A, B) Min(A, B)
#define ClampMin(A, B) Max(A, B)

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
/// cjk: Point Type Definitions 

typedef union {
	struct {
		U16 x;
		U16 y;
	};
	U16 v[2];
} Point2U16;

typedef union {
	struct {
		U32 x;
		U32 y;
	};
	U32 v[2];
} Point2U32;

typedef union {
	struct {
		U64 x;
		U64 y;
	};
	U64 v[2];
} Point2U64;

typedef union {
	struct {
		U16 x;
		U16 y;
		U16 z;
	};
	U16 v[3];
} Point3U16;

typedef union {
	struct {
		U32 x;
		U32 y;
		U32 z;
	};
	U32 v[3];
} Point3U32;

typedef union {
	struct {
		U64 x;
		U64 y;
		U64 z;
	};
	U64 v[3];
} Point3U64;

#define p2_U16(x, y)  	point_2u16((x), (y))
#define p2_U32(x, y) 	point_2u32((x), (y)) 
#define p2_U64(x, y) 	point_2u64((x), (y)) 

#define Point3_U16(x, y, z)  	point_3u16((x), (y), (z))
#define Point3_U32(x, y, z)  	point_3u32((x), (y), (z))
#define Point3_U64(x, y, z)  	point_3u64((x), (y), (z))

Point2U16 point_2u16(U16 x, U16 y);
Point2U32 point_2u32(U32 x, U32 y);
Point2U64 point_2u64(U64 x, U64 y);

Point3U16 point_3u16(U16 x, U16 y, U16 z);
Point3U32 point_3u32(U32 x, U32 y, U32 z);
Point3U64 point_3u64(U64 x, U64 y, U64 z);



///////////////////////////////////////
/// cjk: Vector Types and Functions 

typedef union {
	struct {
		F32 x;
		F32 y;
	};
	F32 v[2];
} Vec2F32;

typedef union {
	struct {
		F64 x;
		F64 y;
	};
	F64 v[2];
} Vec2F64;

typedef union {
	struct {
		F32 x;
		F32 y;
		F32 z;
	};
	F32 v[3];
} Vec3F32;


typedef union {
	struct {
		F64 x;
		F64 y;
		F64 z;
	};
	F64 v[3];
} Vec3F64;

#define Vec2_F32(x, y)		vec_2f32((x), (y))
#define Vec2_F64(x, y)		vec_2f64((x), (y))

#define Vec3_F32(x, y, z)	vec_3f32((x), (y), (z))
#define Vec3_F64(x, y, z)	vec_3f64((x), (y), (z))


Vec2F32 vec_2f32(F32 x, F32 y);
Vec2F64 vec_2f64(F64 x, F64 y);

Vec3F32 vec_3f32(F32 x, F32 y, F32 z);
Vec3F64 vec_3f64(F64 x, F64 y, F64 z);

F64 dot_vec2f32(Vec2F32 v1, Vec2F32 v2);
F64 dot_vec2f64(Vec2F64 v1, Vec2F64 v2);

F64 dot_vec3f32(Vec3F32 v1, Vec3F32 v2);
F64 dot_vec3f64(Vec3F64 v1, Vec3F64 v2);

///////////////////////////////////////
/// cjk: Range Definitions 

typedef union {
	struct {
		U32 min;
		U32 max;
	};
	U32 v[2];
} Rng1U32;

typedef union {
	struct {
		F32 min;
		F32 max;
	};
	F32 v[2];
} Rng1F32;

typedef union {
	struct {
		U64 min;
		U64 max;
	};
	U64 v[2];
} Rng1U64;

typedef union {
	struct {
		F64 min;
		F64 max;
	};
	F64 v[2];
} Rng1F64;

#define Rng1_U32(min, max) 	rng_1u32((min), (max))
#define Rng1_F32(min, max) 	rng_1f32((min), (max))
#define Rng1_U64(min, max) 	rng_1u64((min), (max))
#define Rng1_F64(min, max) 	rng_1f64((min), (max))

Rng1U32 rng_1u32(U32 min, U32 max);
Rng1F32 rng_1f32(F32 min, F32 max);
Rng1U64 rng_1u64(U64 min, U64 max);
Rng1F64 rng_1f64(F64 min, F64 max);

U64 dim_r1u64(Rng1U64 range);


///////////////////////////////////////
/// cjk: Rect Definitions 

typedef union{
	struct{
		U16 x;
		U16 y;
		U16 width;
		U16 height;
	};
	U16 v[4];
} RectU16;

typedef union{
	struct{
		U32 x;
		U32 y;
		U32 width;
		U32 height;
	};
	U32 v[4];
} RectU32;

typedef union{
	struct{
		U64 x;
		U64 y;
		U64 width;
		U64 height;
	};
	U64 v[4];
} RectU64;

typedef union{
	struct{
		F32 x;
		F32 y;
		F32 width;
		F32 height;
	};
	F32 v[4];
} RectF32;

typedef union{
	struct{
		F64 x;
		F64 y;
		F64 width;
		F64 height;
	};
	F64 v[4];
} RectF64;



#define Rect_U16(x, y, w, h) 	rect_u16((x), (y), (w), (h))
#define Rect_U32(x, y, w, h) 	rect_u32((x), (y), (w), (h))
#define Rect_U64(x, y, w, h) 	rect_u64((x), (y), (w), (h))

#define Rect_F32(x, y, w, h) 	rect_f32((x), (y), (w), (h))
#define Rect_F64(x, y, w, h) 	rect_f64((x), (y), (w), (h))

RectU16 rect_u16(U16 x, U16 y, U16 width, U16 height);
RectU32 rect_u32(U32 x, U32 y, U32 width, U32 height);
RectU64 rect_u64(U64 x, U64 y, U64 width, U64 height);

RectF32 rect_f32(F32 x, F32 y, F32 width, F32 height);
RectF64 rect_f64(F64 x, F64 y, F64 width, F64 height);


///////////////////////////////////////
/// cjk: PRNG Definitions 
/// https://en.wikipedia.org/wiki/Mersenne_Twister

// TODO: (cjk) implement Mersenne Twiser psuedo random number generation


///////////////////////////////////////
/// cjk: Color Definitions 

typedef union{
	struct{
		U8 r;
		U8 g;
		U8 b;
		U8 a;
	};
	U32 c;
}ColorRGBA;

typedef union{
	struct{
		U8 b;
		U8 g;
		U8 r;
		U8 a;
	};
	U32 c;
}ColorBGRA;

#define RGB(R,G,B)	color_rgba((U8)(R), (U8)(G), (U8)(B), (U8)255) 
#define RGBA(R,G,B,A)	color_rgba((U8)(R), (U8)(G), (U8)(B), (U8)(A))

// Color pallette
#define COLOR_WHITE 	RGB(255,255,255)
#define COLOR_BLACK 	RGB(0,0,0)
#define COLOR_RED   	RGB(255,0,0) 
#define COLOR_GREEN	RGB(0,255,0) 
#define COLOR_BLUE 	RGB(0, 0, 255) 
#define COLOR_YELLOW 	RGB(255,255,0)
#define COLOR_MAGENTA	RGB(255,0,255) 
#define COLOR_CYAN	RGB(0,255,255) 

ColorRGBA color_rgba(U8 r, U8 g, U8 b, U8 a);
ColorBGRA color_rgba_to_bgra(ColorRGBA color);


///////////////////////////////////////
/// cjk: Arena Definitions 

typedef struct {
	U8 *mem_ptr;
	U64 next_free;
	U64 capacity;
} Arena;

typedef struct {
	Arena *arena;
	U64 original_position;
} ScratchArena;

#define DEFAULT_ARENA_SIZE MB(64)
#define TEMP_ARENA_SIZE KB(4)

#define ArenaPushArray(arena, type, count) 	(type *)arena_push((arena), sizeof(type) * (count))
#define ArenaPushArrayZero(arena, type, count)	(type *)arena_push_zero((arena), sizeof(type) * (count))
#define ArenaPushStruct(arena, type) 		ArenaPushArray(arena, type, (1))
#define ArenaPushStructZero(arena, type) 	ArenaPushArrayZero(arena, type, (1))


#define ScratchArenaScope(arena_ptr)					       \
  for (ScratchArena _scratch_ = scratch_arena_begin(arena_ptr);		       \
       _scratch_.arena != NULL;						       \
       scratch_arena_end(_scratch_), _scratch_.arena = NULL)


/* arena helper functions */
U64 arena_get_position(Arena *arena);
Arena *arena_alloc();
Arena *arena_alloc_with_capacity(U64 size);
void arena_release(Arena *arena);
void *arena_push(Arena *arena, U64 size);
void *arena_push_zero(Arena *arena, U64 size);
void arena_pop(Arena *arena, U64 size);
void arena_pop_to(Arena *arena, U64 pos);
ScratchArena scratch_arena_begin(Arena *arena);
void scratch_arena_end(ScratchArena scratch);

///////////////////////////////////////
/// cjk: Thread Local Arena Definition

// TODO: (Cooper) add thread local scratch arenas
thread_static Arena* tl_scratch_arena[2];
void thread_local_scratch_arena_init(){
	NotImplemented;
}


//////////////////////////////////////
/// cjk: String Definition

typedef U32 Str8_MatchFlags;
enum {
	Str8_MatchRightSideSloppy = (1 << 0),
	Str8_MatchCaseInsensitive = (1 << 1),
};

typedef struct {
	U8 *str;
	U64 size;
} Str8;

#define Str8Lit(S) str8((U8 *)(S), sizeof(S) - 1)
#define Str8VArg(s) (U32)(s).size, (s).str

// C string helpers
U64 cstring_length(const char *c);
Str8 cstring_to_str8(const char *c);
char* str8_to_cstring(Arena* arena, Str8 str);

// String helpers
U8 upper_from_char(U8 chr);
Str8 str8_skip_last_slash(Str8 str);
inline U8 str8_get(Str8 str, U64 idx);
Str8 integer_to_str8(Arena* arena, S64 integer);


// Str8 output
void str8_printf(FILE *file_ptr, const char *format, ...);
Str8 str8_pushf(Arena *arena, const char *format, ...);

// String functions
Str8 str8(U8 *str, U64 length);
Str8 str8_substr(Str8 s1, Rng1U64 range);
Str8 str8_concat(Arena *arena, Str8 s1, Str8 s2);
Str8 str8_copy(Arena *arena, Str8 s1);
B32 str8_cmp(Str8 s1, Str8 s2);
B32 str8_match(Str8 s1, Str8 s2, Str8_MatchFlags flags);

//////////////////////////////////////
/// cjk: String List Definitions

typedef struct Str8Node Str8Node;
struct Str8Node {
	Str8Node *next;
	Str8 string;
};

typedef struct {
	Str8Node *first;
	Str8Node *last;
	U64 count;
	U64 total_size;
} Str8List;

Str8List str8_list();
Str8Node *str8_list_push_node(Str8List *list, Str8Node *node);
Str8Node *str8_list_push_node_front(Str8List *list, Str8Node *node);
Str8Node *str8_list_push(Arena *arena, Str8List *list, Str8 string);
Str8Node *str8_list_push_front(Arena *arena, Str8List *list, Str8 string);

//////////////////////////////////////
/// cjk: String Array Definitions 
typedef struct {
	Str8 *array;
	U64 count;
	U64 capacity;
} Str8Array;

Str8Array str8_array_reserve(Arena *arena, U64 capacity);
Str8Array str8_tokenize_array(Arena *arena, Str8 string, Str8 delimiters);

///////////////////////////////////////
/// cjk: Hashing Definitions

#define DEFAULT_HASH_MAP_SIZE KB(4)
#define DEFAULT_HASH_COLLISION_SIZE KB(1)

typedef struct {
	void *data;
	Str8 key;
	U64 next_data;
	B32 has_next_data;
} HashBucket;

typedef struct {
	Arena *arena;
	U64 next_free_collision_bucket;
	HashBucket data[DEFAULT_HASH_MAP_SIZE];
	HashBucket collisions[DEFAULT_HASH_COLLISION_SIZE];
} HashMap;

HashMap *hash_map_init(Arena *arena);
void *hash_map_get(HashMap *hash_map, Str8 key);
void hash_map_put(HashMap *hash_map, Str8 key, void *data);

// Hashing functions
U64 hash_fnv1a_u64(const void *in_ptr, U64 num_bytes);


///////////////////////////////////////
/// cjk: Profiling Definitions

static clock_t global_profiling_begin, global_profiling_end;

#define ProfileFunc() 	Profile(__func__)
#define Profile(description)	DeferLoop(profile_begin(), profile_end(Str8Lit(description)))

void profile_begin();
void profile_end(Str8 description);

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


#ifdef BASE_ENABLE_OS
///////////////////////////////////////
/// cjk: OS API Definitions

# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <execinfo.h>
# include <signal.h>

typedef U32 OS_AccessFlags;
enum {
	OS_AccessFlag_Read 		= (1 << 0),
	OS_AccessFlag_Write 		= (1 << 1),
	OS_AccessFlag_Execute 		= (1 << 2),
	OS_AccessFlag_Append 		= (1 << 3),
	OS_AccessFlag_ShareRead 	= (1 << 4),
	OS_AccessFlag_ShareWrite 	= (1 << 5),
	OS_AccessFlag_Inherited 	= (1 << 6)
};

typedef U32 OS_FileIterFlags;
enum {
	OS_FileIterFlag_SkipFolders 	= (1 << 0),
	OS_FileIterFlag_SkipFiles 	= (1 << 1),
	OS_FileIterFlag_SkipHiddenFiles = (1 << 2),
	OS_FileIterFlag_Done 		= (1 << 31),
};

typedef U32 OS_FilePropertyFlag;
enum {
	OS_FilePropertyFlag_IsFolder 	= (1 << 0),
	OS_FilePropertyFlag_IsFile 	= (1 << 1),
};

typedef struct {
	U32 logical_processor_count;
	U64 page_size;
	U64 large_page_size;
	U64 allocation_ganularity;
	Str8 machine_name;
} OS_SystemInfo;

typedef struct {
	U32 pid;
	B32 large_pages_allowed;
	Str8 binary_path;
	Str8 initial_path;
	Str8 user_program_data_path;
	Str8List module_load_paths;
	Str8List environment;
} OS_ProcessInfo;

typedef struct {
	OS_FileIterFlags flags;
	U8 buf[KB(2)];
} OS_FileIter;

typedef struct {
	Str8 name;
	DenseTime created;
	DenseTime modified;
	OS_FilePropertyFlag flags;
} OS_FileProperties;

typedef struct {
	Str8 path;
	OS_FileProperties props;
} OS_FileInfo;

typedef U64 OS_Handle;


// TODO: Finish os implementation
//	1) OS entry point - DONE
//	2) File operations
//	3) System and process info
//	4) OS time info
//	5) Memory Allocation
//	6) Directory iteration and creation
//	7) file memory maps
//	8) Large page allocations
//	9) DLL support

// OS entry point
void os_entry_point(U32 argc, U8 **argv);

// File operations
OS_Handle os_file_open(Str8 path, OS_AccessFlags props);
void os_file_close(OS_Handle file_handle);
S64 os_file_read_data(OS_Handle file_handle, Rng1U64 range, void *out_data);
OS_FileProperties os_properties_from_file_handle(OS_Handle file_handle);
B32 os_file_delete_at_path(Str8 path);
B32 os_copy_file_path(Str8 src, Str8 dest);
Str8 os_full_path_from_rel_path(Arena *arena, Str8 rel_path);
B32 os_file_path_exists(Str8 path);
B32 os_folder_path_exists(Str8 path);
OS_FileProperties os_properties_from_file_path(Str8 path);

// File map operations
OS_Handle os_file_map_open(OS_Handle file_handle, OS_AccessFlags flags);
void os_file_map_close(OS_Handle map);
void *os_file_map_view_open(OS_Handle map, OS_AccessFlags flags, Rng1U64 range);
void os_file_map_view_close(OS_Handle map, void *ptr, Rng1U64 range);

// Directory iteration
OS_FileIter *os_file_iter_begin(Arena *arena, Str8 path,
				OS_FileIterFlags flags);
B32 os_file_iter_next(Arena *arena, OS_FileIter *iter, OS_FileInfo *info_out);
void os_file_iter_end(OS_FileIter *iter);

// Directory creation
B32 os_make_directory(Str8 path);

// System and process info
Str8 os_get_current_path(Arena *arena);
OS_SystemInfo os_get_system_info();

// OS memory allocation
void *os_reserve_memory(U64 size);
void os_commit_memory(void *ptr, U64 size);
void os_decommit_memory(void *ptr, U64 size);
void os_release_memory(void *ptr, U64 size);

// OS time
U64 os_now_microseconds();
U32 os_now_unix();
DateTime os_now_universal_time();
DateTime os_universal_time_from_local(DateTime *local_time);
DateTime os_local_time_from_universal(DateTime *universal_time);
void os_sleep_milliseconds(U64 msec);

#endif // BASE_ENABLE_OS

///////////////////////////////////////
/// cjk: CSV Parser Definitions 

typedef struct {
	Str8 delimiters;
	B32 headers_enabled;
} CSV_Config;

typedef struct {
	Str8List *list;
	U64 index;
} CSV_Row;

typedef struct {
	FILE *file_ptr;
	Arena *arena;
	CSV_Config config;
	HashMap column_hash_map;
	CSV_Row current_row;
	ScratchArena row_scratch_arena;
	U8 row_buffer[KB(16)];
	U64 rows;
	U64 columns;
} CSV;

#define EachCSVRow(csv_ptr, row_ptr) 	(CSV_Row *row_ptr = csv_next_row(csv_ptr); row_ptr != NULL; row_ptr = csv_next_row(csv_ptr))

CSV csv_init(Arena *arena, CSV_Config config, char *file_path);
CSV_Row *csv_next_row(CSV *csv);
void csv_row_parse(CSV *csv, Str8 raw_row);


///////////////////////////////////////
/// cjk: Window API Definitions 

#ifdef BASE_ENABLE_WINDOW

# include <X11/Xlib.h>
# include <math.h>
# if defined(_POSIX_SHARED_MEMORY_OBJECTS) && (_POSIX_SHARED_MEMORY_OBJECTS >0)
#  define HAS_SYS_SHM 1
#  include <sys/ipc.h>
#  include <sys/shm.h>
#  include <X11/extensions/XShm.h>
# else
#  define HAS_SYS_SHM 0
# endif


typedef U32 WM_WindowCfgUpdate;
enum{
	WM_WindowCfgUpdate_X		= (1<<0),
	WM_WindowCfgUpdate_Y		= (1<<1),
	WM_WindowCfgUpdate_Width	= (1<<2),
	WM_WindowCfgUpdate_Height	= (1<<3),
	WM_WindowCfgUpdate_BorderWidth	= (1<<4),
	WM_WindowCfgUpdate_Sibiling	= (1<<5),
	WM_WindowCfgUpdate_StackMode	= (1<<6),
};


typedef U64 WM_EventFlag;
enum{
	WM_Event_Keyboard_KeyPress		= (1ULL<<0),
	WM_Event_Keyboard_KeyRelease		= (1ULL<<1),
	WM_Event_Mouse_ButtonPress		= (1ULL<<2),
	WM_Event_Mouse_ButtonRelease		= (1ULL<<3),
	WM_Event_Mouse_MotionNotify		= (1ULL<<4),
	WM_Event_Window_EnterNotify		= (1ULL<<5),
	WM_Event_Window_LeaveNotify		= (1ULL<<6),
	WM_Event_Focus_FocusIn			= (1ULL<<7),
	WM_Event_Focus_FocusOut			= (1ULL<<8),
	WM_Event_KeyMap_KeymapNotify		= (1ULL<<9),
	WM_Event_Exposure_Expose		= (1ULL<<10),
	WM_Event_Exposure_GraphicsExpose	= (1ULL<<11),
	WM_Event_Exposure_NoExpose		= (1ULL<<12),
	WM_Event_Structure_CirculateRequest	= (1ULL<<13),
	WM_Event_Strucutre_ConfigureRequest	= (1ULL<<14),
	WM_Event_Structure_MapRequest		= (1ULL<<15),
	WM_Event_Structure_ResizeRequest	= (1ULL<<16),
	WM_Event_WindowState_CirculateNotify	= (1ULL<<17),
	WM_Event_WindowState_ConfigureNotify	= (1ULL<<18),
	WM_Event_WindowState_CreateNotify	= (1ULL<<19),
	WM_Event_WindowState_DestroyNotify	= (1ULL<<20),
	WM_Event_WindowState_GravityNotif	= (1ULL<<21),
	WM_Event_WindowState_MapNotify		= (1ULL<<22),
	WM_Event_WindowState_MappingNotify	= (1ULL<<23),
	WM_Event_WindowState_ReparentNotify	= (1ULL<<24),
	WM_Event_WindowState_UnmapNotify	= (1ULL<<25),
	WM_Event_WindowState_VisibilityNotify	= (1ULL<<26),
	WM_Event_Colormap_ColormapNotify	= (1ULL<<27),
	WM_Event_ClientComm_ClientMessage	= (1ULL<<28),
	WM_Event_ClientComm_PropertyNotify	= (1ULL<<29),
	WM_Event_ClientComm_SelectionClear	= (1ULL<<30),
	WM_Event_ClientComm_SelectionNotify	= (1ULL<<31),
	WM_Event_ClientComm_SelectionRequest	= (1ULL<<32),
};

// Maps our window manager events to x11 event request
typedef struct{
	WM_EventFlag flag;	
	long event_mask;
}WM_EventMaskMap;

global WM_EventMaskMap wm_event_mask_map[] = {
	{WM_Event_Keyboard_KeyPress		,KeyPressMask},
	{WM_Event_Keyboard_KeyRelease		,KeyReleaseMask},
	{WM_Event_Mouse_ButtonPress		,ButtonPressMask},
	{WM_Event_Mouse_ButtonRelease		,ButtonReleaseMask},
	{WM_Event_Mouse_MotionNotify		,PointerMotionMask},
	{WM_Event_Window_EnterNotify		,EnterWindowMask},
	{WM_Event_Window_LeaveNotify		,LeaveWindowMask},
	{WM_Event_Focus_FocusIn			,FocusChangeMask},
	{WM_Event_Focus_FocusOut		,FocusChangeMask},
	{WM_Event_KeyMap_KeymapNotify		,KeymapStateMask},
	{WM_Event_Exposure_Expose		,ExposureMask},
	{WM_Event_Exposure_GraphicsExpose	,GCGraphicsExposures},
	{WM_Event_Exposure_NoExpose		,GCGraphicsExposures},
	{WM_Event_Structure_CirculateRequest	,SubstructureRedirectMask},
	{WM_Event_Strucutre_ConfigureRequest	,SubstructureRedirectMask},
	{WM_Event_Structure_MapRequest		,SubstructureRedirectMask},
	{WM_Event_Structure_ResizeRequest	,ResizeRedirectMask},
	{WM_Event_WindowState_CirculateNotify	,StructureNotifyMask},
	{WM_Event_WindowState_ConfigureNotify	,StructureNotifyMask},
	{WM_Event_WindowState_CreateNotify	,SubstructureNotifyMask},
	{WM_Event_WindowState_DestroyNotify	,StructureNotifyMask},
	{WM_Event_WindowState_GravityNotif	,StructureNotifyMask},
	{WM_Event_WindowState_MapNotify		,StructureNotifyMask},
	{WM_Event_WindowState_MappingNotify	,NoEventMask},
	{WM_Event_WindowState_ReparentNotify	,StructureNotifyMask},
	{WM_Event_WindowState_UnmapNotify	,StructureNotifyMask},
	{WM_Event_WindowState_VisibilityNotify	,VisibilityChangeMask},
	{WM_Event_Colormap_ColormapNotify	,ColormapChangeMask},
	{WM_Event_ClientComm_ClientMessage	,NoEventMask},
	{WM_Event_ClientComm_PropertyNotify	,PropertyChangeMask},
	{WM_Event_ClientComm_SelectionClear	,NoEventMask},
	{WM_Event_ClientComm_SelectionNotify	,NoEventMask},
	{WM_Event_ClientComm_SelectionRequest	,NoEventMask},
};

typedef struct{
	Display*	display;
	Screen*		screen;
	Arena*		arena;
	Window		window;	
	RectU16		size;
	RectU16		over_size;
	Str8		name;
	GC		graphics_ctx;
	XGCValues	graphics_ctx_values;
#if HAS_SYS_SHM
	XShmSegmentInfo shm_info;
#endif
	XImage* image;	

}WM_Context;


// Open and closing window
WM_Context wm_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, U16 border_width, ColorRGBA border_color,  ColorRGBA background_color);
void wm_close_window(WM_Context* ctx);

// Resizing the window
void wm_resize_window(WM_Context* ctx, U16 width, U16 height);
void wm_move_window(WM_Context* ctx, U16 x, U16 y);
void wm_resize_and_move_window(WM_Context* ctx, RectU16 new_size);
void wm_draw_window(WM_Context* ctx);

// 2d primitive drawing
void wm_draw_rect(WM_Context* ctx, RectF32 rect, ColorRGBA color);
void wm_draw_circle(WM_Context* ctx, Vec2F32 center, F32 radius, ColorRGBA color) ;
void wm_draw_line(WM_Context* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color);
void wm_draw_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color) ;
B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3);
void wm_draw_filled_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color);

// Event handeling
void wm_register_input_events(WM_Context* ctx, WM_EventFlag flags);
U32 wm_num_of_pending_events(WM_Context* ctx);



//////////////////////////////////////
/// cjk: .obj reader Definitions 





#endif // BASE_ENABLE_WINDOW

#endif // BASE_H

// Private function definitions
#ifdef BASE_IMPLEMENTATION

///////////////////////////////////////
/// cjk: Base Integer Functions 

U8 safe_cast_U8(U16 x) {
	Assert(x < max_U8);
	return (U8)x;
}

U16 safe_cast_U16(U32 x) {
	Assert(x < max_U16);
	return (U16)x;
}
U32 safe_cast_U32(U64 x) {
	Assert(x < max_U32);
	return (U32)x;
}

S8 safe_cast_S8(S16 x) {
	Assert(x < max_S8);
	Assert(x > min_S8);
	return (S8)x;
}
S16 safe_cast_S16(S32 x) {
	Assert(x < max_S16);
	Assert(x > min_S16);
	return (S16)x;
}
S32 safe_cast_S32(S64 x) {
	Assert(x < max_S32);
	Assert(x > min_S32);
	return (S32)x;
}

///////////////////////////////////////
/// cjk:  Rect Functions 

RectU16 rect_u16(U16 x, U16 y, U16 width, U16 height){ return (RectU16){{x, y, width, height}}; };
RectU32 rect_u32(U32 x, U32 y, U32 width, U32 height){ return (RectU32){{x, y, width, height}}; };
RectU64 rect_u64(U64 x, U64 y, U64 width, U64 height){ return (RectU64){{x, y, width, height}}; };

RectF32 rect_f32(F32 x, F32 y, F32 width, F32 height){ return (RectF32){{x, y, width, height}}; };
RectF64 rect_f64(F64 x, F64 y, F64 width, F64 height){ return (RectF64){{x, y, width, height}}; };


///////////////////////////////////////
/// cjk:  Range Functions 

Rng1U32 rng_1u32(U32 min, U32 max) {
	Assert(min < max);
	return (Rng1U32){{.min = min, .max = max}};
}

Rng1F32 rng_1f32(F32 min, F32 max) {
	Assert(min < max);
	return (Rng1F32){{.min = min, .max = max}};
}

Rng1U64 rng_1u64(U64 min, U64 max) {
	Assert(min < max);
	return (Rng1U64){{.min = min, .max = max}};
}

Rng1F64 rng_1f64(F64 min, F64 max) {
	Assert(min < max);
	return (Rng1F64){{.min = min, .max = max}};
}

U64 dim_r1u64(Rng1U64 r) { 
	return ((r.max > r.min) ? (r.max - r.min) : 0); 
}

///////////////////////////////////////
/// cjk:  Point Functions 

Point2U16 point_2u16(U16 x, U16 y){ return (Point2U16){{x, y}};}
Point2U32 point_2u32(U32 x, U32 y){ return (Point2U32){{x, y}};}
Point2U64 point_2u64(U64 x, U64 y){ return (Point2U64){{x, y}};}

Point3U16 point_3u16(U16 x, U16 y, U16 z){ return (Point3U16){{x, y, z}};}
Point3U32 point_3U32(U32 x, U32 y, U32 z){ return (Point3U32){{x, y, z}};}
Point3U64 point_3U64(U64 x, U64 y, U64 z){ return (Point3U64){{x, y, z}};}

///////////////////////////////////////
/// cjk:  Vector Functions 

// Initializers
Vec2F32 vec_2f32(F32 x, F32 y){ return (Vec2F32){{x, y}};}
Vec2F64 vec_2f64(F64 x, F64 y){ return (Vec2F64){{x, y}};}

Vec3F32 vec_3f32(F32 x, F32 y, F32 z){ return (Vec3F32){{x, y, z}};}
Vec3F64 vec_3f64(F64 x, F64 y, F64 z){ return (Vec3F64){{x, y, z}};}

// dot products
F64 dot_vec2f32(Vec2F32 v1, Vec2F32 v2){ return (v1.x * v2.x) + (v2.y * v2.y);}
F64 dot_vec2f64(Vec2F64 v1, Vec2F64 v2){ return (v1.x * v2.x) + (v2.y * v2.y);}

F64 dot_vec3f32(Vec3F32 v1, Vec3F32 v2){ return (v1.x * v2.x) + (v2.y * v2.y) + (v1.z * v2.z);}
F64 dot_vec3f64(Vec3F64 v1, Vec3F64 v2){ return (v1.x * v2.x) + (v2.y * v2.y) + (v1.z * v2.z);}


///////////////////////////////////////
/// cjk: Color Functions 

ColorRGBA color_rgba(U8 r, U8 g, U8 b, U8 a){ return (ColorRGBA){{ r, g, b,a}};}

ColorBGRA color_rgba_to_bgra(ColorRGBA color){
	return (ColorBGRA){
		.b = color.b,
		.g = color.g,
		.r = color.r,
		.a = color.a
	};
}


///////////////////////////////////////
/// cjk: Arena Functions

U64 arena_get_position(Arena *arena) {
	Assert(arena != NULL);
	return arena->next_free;
}

Arena *arena_alloc() { 
	return arena_alloc_with_capacity(DEFAULT_ARENA_SIZE); 
}

Arena *arena_alloc_with_capacity(U64 size) {
	Arena *arena = (Arena *)malloc(sizeof(Arena));
	Assert(arena);

	arena->capacity = size;

	arena->mem_ptr = (U8 *)malloc(arena->capacity);
	Assert(arena->mem_ptr);
	AsanPoisonMemoryRegion(arena->mem_ptr, size);

	arena->next_free = 0;
	return arena;
}

void arena_release(Arena *arena) {
	Assert(arena != NULL);
	arena->capacity = 0;
	arena->next_free = 0;
	free(arena->mem_ptr);
	free(arena);
}

void *arena_push(Arena *arena, U64 size) {
	Assert(arena != NULL);
	if (arena->next_free + size <= arena->capacity) {
		void *ptr = arena->mem_ptr + arena->next_free;
		AsanUnpoisonMemoryRegion(ptr, size);
		arena->next_free += size;
		return ptr;
	}
	InvalidPath;
	return NULL;
}

void *arena_push_zero(Arena *arena, U64 size) {
	Assert(arena != NULL);
	if (arena->next_free + size <= arena->capacity) {
		void *ptr = arena->mem_ptr + arena->next_free;
		AsanUnpoisonMemoryRegion(ptr, size);
		arena->next_free += size;
		memset(ptr, 0, size);
		return ptr;
	}
	InvalidPath;
	return NULL;
}

void arena_pop(Arena *arena, U64 size) {
	Assert(arena != NULL);
	U64 pos = arena->next_free - size;
	arena_pop_to(arena, pos);
}

void arena_pop_to(Arena *arena, U64 new_pos) {
	Assert(arena != NULL);

	U64 current_pos = arena_get_position(arena);

	Assert(new_pos <= current_pos);

	U64 asan_poison_size = current_pos - new_pos;
	void *asan_poison_ptr = arena->mem_ptr + new_pos;
	AsanPoisonMemoryRegion(asan_poison_ptr, asan_poison_size);

	arena->next_free = new_pos;
}

ScratchArena scratch_arena_begin(Arena *arena) {
	Assert(arena != NULL);
	return (ScratchArena){.arena = arena, .original_position = arena_get_position(arena)};
}

void scratch_arena_end(ScratchArena scratch) {
	Assert(scratch.arena != NULL);
	arena_pop_to(scratch.arena, scratch.original_position);
}

///////////////////////////////////////
/// cjk: String Functions 

U8 upper_from_char(U8 chr) {
	if (chr > 'A') {
		return chr - 32;
	}
	return chr;
}

Str8 str8(U8 *str, U64 length) {
	Assert(str != NULL);
	return (Str8){str, length};
}

void str8_printf(FILE *file_ptr, const char *format, ...) {
	Assert(file_ptr != NULL);
	Assert(format != NULL);
	va_list args;
	va_start(args, format);
	vfprintf(file_ptr, format, args);
	va_end(args);
}

Str8 str8_pushf(Arena *arena, const char *format, ...) {
	Assert(arena);
	Assert(format);

	va_list args;
	va_start(args, format);

	va_list copy;
	va_copy(copy, args);

	U64 len = vsnprintf(NULL, 0, format, copy);

	U8 *ptr = ArenaPushArray(arena, U8, len + 1);
	vsnprintf((char *)ptr, len + 1, format, args);
	arena_pop(arena, 1);

	va_end(args);
	return (Str8){ptr, len};
}

Str8 str8_skip_last_slash(Str8 str) {
	Assert(str.size > 0);
	Assert(str.str);

	U8 *ptr = str.str + str.size - 1;
	for (; ptr >= str.str; ptr -= 1) {
		if (*ptr == '/' || *ptr == '\\') break;
	}

	if (ptr >= str.str) {
		ptr += 1;
		str.size = (U64)(str.str + str.size - ptr);
		str.str = ptr;
	}
	return str;
}

U8 str8_get(Str8 string, U64 idx) {
	if (idx < string.size) {
		return string.str[idx];
	}
	InvalidPath;
	return '\0';
}

Str8 integer_to_str8(Arena* arena, S64 integer){
	if(integer == 0) return Str8Lit("0");
	B32 is_negative = (integer < 0);
	
	U64 abs_val = is_negative ? (U64)(-integer) : (U64)integer;

	U64 digits = 0;
	U64 divide_by = 1;
	for(int i = abs_val; i > 0; i /= 10) {
		digits++; 
		if(i >= 10) divide_by *= 10;
	}

	if(is_negative) digits ++;
	Str8 result = (Str8){
		.str = ArenaPushArray(arena, U8, digits),
		.size = digits 
	};

	if(is_negative) result.str[0] = '-';

	for EachIndex(i, result.size){
		if(is_negative && i == 0) continue;
		U64 digit = (abs_val/divide_by) % 10;
		result.str[i] = (char)(digit + 48);
		divide_by /= 10;
	}	

	return result;
}

U64 cstring_length(const char *c) {
	Assert(c != NULL);
	U64 length = 0;
	if (c) {
		U8 *p = (U8 *)c;
		for (; *p != 0; p += 1);
		length = (U64)(p - (U8 *)c);
	}
	return length;
}

Str8 cstring_to_str8(const char *c) {
	Assert(c != NULL);
	return (Str8){(U8 *)c, cstring_length(c)};
}

char* str8_to_cstring(Arena* arena, Str8 str){
	Assert(arena);
	Assert(str.size > 0);
	Assert(str.str);

	char* cstring = ArenaPushArray(arena, char,str.size + 1);
	MemoryCopy(cstring, str.str, str.size);
	cstring[str.size] = '\0';
	return cstring;	
}

Str8 str8_concat(Arena *arena, Str8 s1, Str8 s2) {
	Assert(arena != NULL);

	U64 new_size = s1.size + s2.size;
	U8 *character_buffer = ArenaPushArray(arena, U8, new_size);

	MemoryCopy(character_buffer, s1.str, s1.size);
	MemoryCopy(character_buffer + s1.size, s2.str, s2.size);

	return (Str8){ .str = character_buffer, .size = new_size};
}

Str8 str8_copy(Arena *arena, Str8 s1) {
	Assert(arena != NULL);

	Str8 new_string = (Str8){
		.str = ArenaPushArray(arena, U8, s1.size),
		.size = s1.size,
	};

	MemoryCopy(new_string.str, s1.str, s1.size);
	return new_string;
}

Str8 str8_substr(Str8 s1, Rng1U64 range) {
	range.min = ClampMax(range.min, s1.size);
	range.max = ClampMax(range.max, s1.size);

	s1.str += range.min;
	s1.size = dim_r1u64(range);
	return s1;
}

B32 str8_cmp(Str8 s1, Str8 s2) {
	Assert(s1.size > 0);
	Assert(s2.size > 0);
	Assert(s1.str);
	Assert(s2.str);

	if (s1.size != s2.size) return false;
	return MemoryMatch(s1.str, s2.str, s1.size);
}

B32 str8_match(Str8 s1, Str8 s2, Str8_MatchFlags flags) {
	B32 result = 1;

	if (s1.size == s2.size && (flags == 0)) {
		return MemoryMatch(s1.str, s2.str, s1.size);
	}

	else if (s1.size == s2.size || (flags & Str8_MatchRightSideSloppy)) {

		B32 case_insensitive = (flags & Str8_MatchCaseInsensitive);
		U64 size = Min(s1.size, s2.size);

		for EachIndex(i, size) {
			U8 s1_char = s1.str[i];
			U8 s2_char = s2.str[i];

			if (case_insensitive) {
				s1_char = upper_from_char(s1_char);
				s2_char = upper_from_char(s2_char);
			}

			if (s1_char != s2_char) {
				result = 0;
				break;
			}
		}
	}
	return result;
}

///////////////////////////////////////
/// cjk: String List Functions 

Str8List str8_list() {
	return (Str8List){
		.first = NULL,
		.last = NULL,
		.count = 0,
		.total_size = 0,
	};
}

Str8Node *str8_list_push_node(Str8List *list, Str8Node *node) {
	Assert(list != NULL);
	Assert(node != NULL);

	if (list->last == NULL && list->first == NULL) {
		list->last = node;
		list->first = node;
	} else {
		list->last->next = node;
		list->last = node;
	}

	list->count += 1;
	list->total_size += node->string.size;
	return node;
}

Str8Node *str8_list_push_node_front(Str8List *list, Str8Node *node) {
	Assert(list != NULL);
	Assert(node != NULL);

	if (list->last == NULL && list->first == NULL) {
		list->last = node;
		list->first = node;
	} else {
		node->next = list->first;
		list->first = node;
	}

	list->count += 1;
	list->total_size += node->string.size;

	return node;
}

Str8Node *str8_list_push(Arena *arena, Str8List *list, Str8 string) {
	Assert(arena != NULL);
	Assert(list != NULL);

	Str8Node *node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node){.next = NULL, .string = string};

	return str8_list_push_node(list, node);
}

Str8Node *str8_list_push_front(Arena *arena, Str8List *list, Str8 string) {
	Assert(arena != NULL);
	Assert(list != NULL);

	Str8Node *node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node){.next = NULL, .string = string};

	return str8_list_push_node_front(list, node);
}

Str8List *str8_tokenize_list(Arena *arena, Str8 string, Str8 delimiters) {
	Assert(arena != NULL);
	Assert(delimiters.size > 0);

	U64 start_idx = 0;
	Str8List *list = ArenaPushStruct(arena, Str8List);

	for EachIndex(i, string.size) {
		B32 is_delimiter = 0;

		for EachIndex(delim, delimiters.size) {
			if (str8_get(string, i) == str8_get(delimiters, delim)) {
				is_delimiter = 1;
				break;
			}
		}

		if (is_delimiter) {
			Str8 token = str8_substr(string, Rng1_U64(start_idx, i));
			start_idx = i + 1;
			str8_list_push(arena, list, token);
		}
	}

	if (start_idx <= string.size) { // Push final token
		Str8 token = str8_substr(string, Rng1_U64(start_idx, string.size));
		str8_list_push(arena, list, token);
	}

	return list;
}

///////////////////////////////////////
/// cjk: String Array Functions 

Str8Array str8_array_reserve(Arena *arena, U64 capacity){
	NotImplemented;
}
Str8Array str8_tokenize_array(Arena *arena, Str8 string, Str8 delimiters){
	NotImplemented;
}

///////////////////////////////////////
/// cjk: HashMap Functions 

HashMap *hash_map_init(Arena *arena) {
	HashMap *hashmap = ArenaPushStructZero(arena, HashMap);
	hashmap->arena = arena;
	hashmap->next_free_collision_bucket = U64Lit(0);
	return hashmap;
}

void *hash_map_get(HashMap *hash_map, Str8 key) {
	Assert(hash_map);
	Assert(key.size > 0);
	Assert(key.str);

	U64 hash = hash_fnv1a_u64(key.str, key.size);
	hash = hash % DEFAULT_HASH_MAP_SIZE;

	HashBucket bucket = hash_map->data[hash];

	if (str8_cmp(key, bucket.key)) return bucket.data;

	while (bucket.has_next_data) {
		if (str8_cmp(key, bucket.key)) return bucket.data;
		bucket = hash_map->data[bucket.next_data];
	}
	return NULL;
}

void hash_map_put(HashMap *hash_map, Str8 key, void *data) {
	Assert(hash_map);
	Assert(data);
	Assert(key.size > 0);
	Assert(key.str);

	local_persist U64 collisions = 0;
	U64 hash = hash_fnv1a_u64(key.str, key.size);
	hash = hash % DEFAULT_HASH_MAP_SIZE;

	HashBucket *bucket = &hash_map->data[hash];

	// if bucket is empty
	if (bucket->data == NULL) {
		bucket->data = data;
		bucket->key = str8_copy(hash_map->arena, key);
		bucket->has_next_data = false;
		bucket->next_data = 0;
		return;
	}

	printf("Collisions: %lu\n", collisions);
	//
	// if this assert goes off consider increasing the size of the hashmap
	// or changing hash function / choosing a different seed
	Assert(collisions < 1000);
	collisions += 1;

	// traverse collision array until empty bucket is found
	HashBucket *prev = NULL;
	while (bucket->data != NULL) {
		prev = bucket;
		if (bucket->has_next_data) {
			bucket = &hash_map->collisions[bucket->next_data];
			continue;
		}
		break;
	}

	// set previous buckets next data to the current bucket
	prev->has_next_data = true;
	prev->next_data = hash_map->next_free_collision_bucket;

	// insert data
	bucket = &hash_map->collisions[hash_map->next_free_collision_bucket];
	bucket->data = data;
	bucket->key = str8_copy(hash_map->arena, key);
	bucket->has_next_data = false;
	bucket->next_data = 0;

	hash_map->next_free_collision_bucket += 1;
	Assert(hash_map->next_free_collision_bucket <= DEFAULT_HASH_COLLISION_SIZE);
}

// Hashing functions
U64 hash_fnv1a_u64(const void *in_ptr, U64 num_bytes) {
	Assert(num_bytes > 0);
	Assert(in_ptr);

	const U8 *byte_arr = (const U8 *)in_ptr;

	// seeds for FNV-1a hashing do not touch
	U64 fnv_prime = U64Lit(0x00000100000001b3);
	U64 fnv_hash = U64Lit(0xcbf29ce484222325);

	for EachIndex(i, num_bytes) {
		fnv_hash ^= byte_arr[i];
		fnv_hash *= fnv_prime;
	}

	return fnv_hash;
}

///////////////////////////////////////
/// cjk: Profiling Functions

void profile_begin() { 
	global_profiling_begin = clock(); 
}

void profile_end(Str8 message) {
	global_profiling_end = clock();
	F64 time_spent = (F64)(global_profiling_end - global_profiling_begin) / CLOCKS_PER_SEC;
	str8_printf(stderr, "[PROFILING] %.*s: %.4f(s)\n", Str8VArg(message), time_spent);
}

///////////////////////////////////////
/// cjk: Time Functions

DenseTime densetime_from_datetime(DateTime time) {
	DenseTime result = 0;
	result += time.year;
	result *= 12;
	result += time.month;
	result *= 31;
	result += time.day;
	result *= 24;
	result += time.hour;
	result *= 60;
	result += time.min;
	result *= 61;
	result += time.sec;
	result *= 1000;
	result += time.mil_sec;
	return result;
}

DateTime datetime_from_densetime(DenseTime time) {
	DateTime result = {0};
	result.mil_sec = time % 1000;
	time /= 1000;
	result.sec = time % 61;
	time /= 61;
	result.min = time % 60;
	time /= 60;
	result.hour = time % 24;
	time /= 24;
	result.day = time % 31;
	time /= 31;
	result.month_num = time % 12;
	time /= 12;
	Assert(time < max_U32);
	result.year = (U32)time;
	return result;
}

DateTime datetime_from_unixtime(U64 unix_time) {
	DateTime date 	= {0};
	date.year 	= 1970;
	date.day 	= 1 + (unix_time / 86400);
	date.min 	= (U32)unix_time % 60;
	date.sec 	= (U32)(unix_time / 60) % 60;
	date.hour 	= (U32)(unix_time / 3600) % 24;

	for(;;){
		for (date.month = 0; date.month <= 12; ++date.month) {
			U64 c = 0;
			switch (date.month) {
				case Month_Jan: c = 31; break;
				case Month_Feb: {
					if ((date.year % 4 == 0) && ((date.year % 100) != 0 || (date.year % 400) == 0)) c = 29;
					else c = 28;
				} break;
				case Month_Mar: c = 31; break;
				case Month_Apr: c = 30; break;
				case Month_May: c = 31; break;
				case Month_Jun: c = 30; break;
				case Month_Jul: c = 31; break;
				case Month_Aug: c = 31; break;
				case Month_Sep: c = 31; break;
				case Month_Oct: c = 31; break;
				case Month_Nov: c = 31; break;
				case Month_Dec: c = 31; break;
				default: InvalidPath;
			}

			if (date.day <= c) goto exit;	
			date.day -= c;
		}
		++date.year;
	}
	exit:;
	return date;
}

///////////////////////////////////////
/// cjk: CSV Functions

CSV csv_init(Arena *arena, CSV_Config config, char *file_path) {
	Assert(arena);
	Assert(file_path);
	CSV csv_parser = {.file_ptr = fopen(file_path, "rb"),
		    .config = config,
		    .arena = arena,
		    .column_hash_map = {0},
		    .current_row = {0},
		    .rows = 0,
		    .columns = 0};

	Assert(csv_parser.file_ptr);

	return csv_parser;
}

CSV_Row *csv_next_row(CSV *csv) {
	Assert(csv != NULL);

	char *err =
	fgets((char *)csv->row_buffer, sizeof(csv->row_buffer), csv->file_ptr);
	if (err == NULL) return NULL;

	Str8 raw_row = cstring_to_str8((char *)csv->row_buffer);
	csv_row_parse(csv, raw_row);

	return &csv->current_row;
}

void csv_row_parse(CSV *csv, Str8 raw_row) {
	Assert(csv != NULL);

	U64 start_idx = 0;
	Str8 delimiters = csv->config.delimiters;
	Str8 quotes = Str8Lit("\"\'");

	ScratchArenaScope(csv->arena) {
		csv->current_row.list = ArenaPushStructZero(csv->arena, Str8List);
		*csv->current_row.list = str8_list();

		B32 is_quoted = 0;

		for EachIndex(i, raw_row.size) {
			U8 current_char = str8_get(raw_row, i);

			B32 is_quote_char = 0;
			for EachIndex(quote, quotes.size) {
				if (current_char == str8_get(quotes, quote)) {
					is_quote_char = 1;
					break;
				}
			}

			if (is_quote_char) {
				is_quoted = !is_quoted;
				continue;
			}

			if (is_quoted)break;

			B32 is_delimiter = 0;
			for EachIndex(delim, delimiters.size) {
				if (current_char == str8_get(delimiters, delim)) {
					is_delimiter = 1;
					break;
				}
			}

			if (is_delimiter) {
				Str8 token = str8_substr(raw_row, Rng1_U64(start_idx, i));
				str8_list_push(csv->arena, csv->current_row.list, token);
				start_idx = i + 1;
			}
		}

		if (start_idx <= raw_row.size) { // Push final token
			Str8 token = str8_substr(raw_row, Rng1_U64(start_idx, raw_row.size));
			str8_list_push(csv->arena, csv->current_row.list, token);
		}
	}
}

#ifdef BASE_ENABLE_OS
///////////////////////////////////////
/// cjk: OS API Functions 

# if OS_LINUX

///////////////////////////////////////
/// cjk: Linux API Helper Functions 

DateTime os_lnx_datetime_from_timespec(struct timespec time){
	struct tm tm_time;
	void* err = gmtime_r(&time.tv_sec, &tm_time);
	Assert(err != NULL);

	DateTime result = (DateTime){
		.sec = tm_time.tm_sec,
		.min = tm_time.tm_min,
		.hour = tm_time.tm_hour,
		.day = tm_time.tm_mday - 1,
		.month_num = tm_time.tm_mon,
		.year = tm_time.tm_year + 1900,
		.week_day = tm_time.tm_wday,
		.micro_sec = (time.tv_nsec / Thousand(1)),
		.mil_sec = (time.tv_nsec / Million(1))
	};

	return result;
}


void os_lnx_signal_handler(int sig, siginfo_t *info, void *arg) {
	local_persist void *bt_buffer[KB(4)];
	U64 bt_count = backtrace(bt_buffer, ArrayCount(bt_buffer));
	
	fprintf(stderr, "[Process Recieved Signal: %s (%d)]\n", strsignal(sig), sig);
	if(errno != 0){
		int err = errno;
		const char* error_description = strerrordesc_np(err);
		const char* error_name = strerrorname_np(err);
		fprintf(stderr, "[Errno (%d) %s: %s]", err, error_name, error_description);
	}

	for EachIndex(i, bt_count) {

		Dl_info dl_info = {0};
		dladdr(bt_buffer[i], &dl_info);

		char cmd[KB(2)];
		snprintf(cmd, sizeof(cmd), "llvm-symbolizer --relative-address -f -e %s %lu", dl_info.dli_fname, (unsigned long)bt_buffer[i] - (unsigned long)dl_info.dli_fbase);

		FILE *f = popen(cmd, "r");

		if (f) {
			char func_name[256], file_name[256];
			if (fgets(func_name, sizeof(func_name), f) && fgets(file_name, sizeof(file_name), f)) {

				Str8 func = cstring_to_str8(func_name);
				if (func.size > 0) func.size -= 1;

				Str8 module = str8_skip_last_slash(cstring_to_str8(dl_info.dli_fname));
				Str8 file = str8_skip_last_slash(cstring_to_str8(file_name));
				if (file.size > 0) file.size -= 1;

				B32 no_func = str8_match(func, Str8Lit("??"), Str8_MatchRightSideSloppy);
				B32 no_file = str8_match(file, Str8Lit("??"), Str8_MatchRightSideSloppy);

				if (no_func) func = (Str8){0};
				if (no_file) file = (Str8){0};

				fprintf(stderr, "%ld. [0x%016lx] %.*s%s%.*s %.*s\n", i + 1, (unsigned long)bt_buffer[i], Str8VArg(module), (!no_file || !no_file) ? ", " : "", Str8VArg(func), Str8VArg(file));
			}
			pclose(f);
		} else {
			fprintf(stderr, "%ld. [0x%016lx] %s\n", i + 1,(unsigned long)bt_buffer[i], dl_info.dli_fname);
		}
	}
	exit(0);
}

///////////////////////////////////////
/// cjk: OS Linux API Functions 

void os_entry_point(U32 argc, U8 **argv) {
	(void)argc;
	(void)argv;

	U32 pid = (U32)getpid();
	fprintf(stderr, "[Process started with pid: %d]\n", pid);
	struct sigaction handler = {.sa_sigaction = &os_lnx_signal_handler,
			      .sa_flags = SA_SIGINFO};
	sigfillset(&handler.sa_mask);
	sigaction(SIGILL, &handler, NULL);
	sigaction(SIGTRAP, &handler, NULL);
	sigaction(SIGABRT, &handler, NULL);
	sigaction(SIGQUIT, &handler, NULL);
	sigaction(SIGSEGV, &handler, NULL);
	sigaction(SIGFPE, &handler, NULL);
	sigaction(SIGBUS, &handler, NULL);
}

// File operations
OS_Handle os_file_open(Str8 path, OS_AccessFlags flags) {
	Arena* temp = arena_alloc_with_capacity(TEMP_ARENA_SIZE);
	Assert(temp);

	int open_flags = 0;
	
	if(flags & OS_AccessFlag_Read && flags & OS_AccessFlag_Write){
		open_flags |= O_RDWR;
	}else if (flags & OS_AccessFlag_Read){
		open_flags |= O_RDONLY;
	}else if (flags & OS_AccessFlag_Write){
		open_flags |= O_WRONLY;
	}
	
	const char* cstr_file_path = str8_to_cstring(temp, path);
	
	int fd = open(cstr_file_path, open_flags);
	Assert(fd != -1);

	OS_Handle handle = fd;
	arena_release(temp);
	return handle;

	/* TODO: (cjk): add more open file flags
	if (flags & OS_AccessFlag_Execute){}
	if (flags & OS_AccessFlag_Append){}
	if (flags & OS_AccessFlag_ShareRead){}
	if (flags & OS_AccessFlag_ShareWrite){}
	if (flags & OS_AccessFlag_Inherited){}
	*/

}

void os_file_close(OS_Handle file_handle) {
	S32 err = close(file_handle);
	Assert(err != -1);
}

S64 os_file_read_data(OS_Handle file_handle, Rng1U64 range, void *out_data) {
	U64 count = dim_r1u64(range);
	U64 offset = range.min;

	S32 result = 0;
	Assert(count != 0);

	result = lseek(file_handle, offset, SEEK_SET);
	Assert(result != -1);

	result = read(file_handle, out_data, count);
	Assert(result != -1);

	return result;
}

OS_FileProperties os_properties_from_file_handle(OS_Handle file_handle) {
	OS_FileProperties props = {0};
	struct stat statbuf;
	Arena* temp = arena_alloc_with_capacity(TEMP_ARENA_SIZE);

	S32 err = fstat(file_handle, &statbuf);
	Assert(err != -1);	

	DateTime last_modified = os_lnx_datetime_from_timespec(statbuf.st_mtim);
	props.modified = densetime_from_datetime(last_modified);

	if(S_ISREG(statbuf.st_mode)){
		props.flags = OS_FilePropertyFlag_IsFile;
	}else if(S_ISDIR(statbuf.st_mode)){
		props.flags = OS_FilePropertyFlag_IsFolder;
	}
		
	Str8 path = Str8Lit("/proc/self/fd/");

	Str8 fd = integer_to_str8(temp, file_handle);
	
	Str8 fd_path = str8_concat(temp, path, fd);
	const char* cstr_fd_path = str8_to_cstring(temp, fd_path);


	Str8 file_name = (Str8){
		.str=ArenaPushArray(temp, U8, 256),
		.size = 256,
	};	

	err = readlink(cstr_fd_path, (char*)file_name.str, file_name.size);
	Assert(err != -1);	
	U32 bytes_read = err;

	file_name.size = bytes_read;
	props.name = file_name;

	arena_release(temp);
	return props;
}

OS_FileProperties os_properties_from_file_path(Str8 path) {
	OS_FileProperties props = {0};
	struct stat statbuf;

	Arena* temp = arena_alloc_with_capacity(TEMP_ARENA_SIZE);
	Assert(temp != NULL);

	char* cstr_file_path = str8_to_cstring(temp, path);

	S32 err = stat(cstr_file_path, &statbuf);
	Assert(err != -1);	

	DateTime last_modified = os_lnx_datetime_from_timespec(statbuf.st_mtim);
	props.modified = densetime_from_datetime(last_modified);

	if(S_ISREG(statbuf.st_mode)){
		props.flags = OS_FilePropertyFlag_IsFile;
	}else if(S_ISDIR(statbuf.st_mode)){
		props.flags = OS_FilePropertyFlag_IsFolder;
	}

	props.name = str8_skip_last_slash(path);

	return props;

}

B32 os_file_delete_at_path(Str8 path) { NotImplemented; }
B32 os_copy_file_path(Str8 src, Str8 dest) { NotImplemented; }
Str8 os_full_path_from_rel_path(Arena *arena, Str8 rel_path) { NotImplemented; }
B32 os_file_path_exists(Str8 path) { NotImplemented; }
B32 os_folder_path_exists(Str8 path) { NotImplemented; }


// File map operations
OS_Handle os_file_map_open(OS_Handle file_handle, OS_AccessFlags flags) {NotImplemented;}
void os_file_map_close(OS_Handle map) { NotImplemented; }
void *os_file_map_view_open(OS_Handle map, OS_AccessFlags flags, Rng1U64 range) {NotImplemented;}
void os_file_map_view_close(OS_Handle map, void *ptr, Rng1U64 range) {NotImplemented;}

// Directory iteration
OS_FileIter *os_file_iter_begin(Arena *arena, Str8 path,OS_FileIterFlags flags) {NotImplemented;}
B32 os_file_iter_next(Arena *arena, OS_FileIter *iter, OS_FileInfo *info_out) {NotImplemented;}
void os_file_iter_end(OS_FileIter *iter) { NotImplemented; }

// Directory creation
B32 os_make_directory(Str8 path) { NotImplemented; }

// System and process info
Str8 os_get_current_path(Arena *arena) { NotImplemented; }
OS_SystemInfo os_get_system_info() { NotImplemented; }

// OS memory allocation
void *os_reserve_memory(U64 size) { NotImplemented; }
void os_commit_memory(void *ptr, U64 size) { NotImplemented; }
void os_decommit_memory(void *ptr, U64 size) { NotImplemented; }
void os_release_memory(void *ptr, U64 size) { NotImplemented; }

// OS time
U64 os_now_microseconds() { NotImplemented; }
U32 os_now_unix() { NotImplemented; }
DateTime os_now_universal_time() { NotImplemented; }
DateTime os_universal_time_from_local(DateTime *local_time) { NotImplemented; }
DateTime os_local_time_from_universal(DateTime *universal_time) {NotImplemented;}
void os_sleep_milliseconds(U64 msec) { NotImplemented; }

# elif OS_WINDOWS
#  error "Windows is currently unsupporrted"
# elif OS_MAC
#  error "Mac is currently unsupported"
# else
#  error "Unknown OS error"
# endif
#endif // BASE_ENABLE_OS


///////////////////////////////////////
/// cjk: Window API Functions 

#ifdef BASE_ENABLE_WINDOW

WM_Context wm_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, U16 border_width, ColorRGBA border_color,  ColorRGBA background_color){
	Assert(arena);	

	// default values
	U16 default_boarder = 10;

	WM_Context result = {0};

	result.arena = arena;
	result.size = win_rect;
	result.name = window_name;

	result.display = XOpenDisplay(NULL);
	result.screen = XDefaultScreenOfDisplay(result.display);
	result.window = XCreateSimpleWindow(result.display, 
			       XDefaultRootWindow(result.display), 
			       win_rect.x, win_rect.y, 
			       win_rect.width, win_rect.height,
			       (border_width == 0)? default_boarder : border_width,
			       border_color.c,
			       background_color.c);

	XSetWindowAttributes attr;
	attr.background_pixmap = None;
	attr.bit_gravity = NorthWestGravity;
	XChangeWindowAttributes(result.display, result.window, CWBackPixmap, &attr);


	U32 screen = DefaultScreen(result.display);

	result.over_size = Rect_U16(0,0, DisplayWidth(result.display, screen), DisplayHeight(result.display, screen));

	Assert(result.display);
	Assert(result.screen);
	Assert(result.window);

#if HAS_SYS_SHM
	printf("[MIT-SHM supported by X11]\n");	


	result.image = XShmCreateImage(result.display,
				DefaultVisualOfScreen(result.screen),
				DefaultDepthOfScreen(result.screen),
				ZPixmap,
				NULL,
				&result.shm_info,
				result.over_size.width, result.over_size.height);

	Assert(result.image);

	U64 total_size = result.image->bytes_per_line * result.image->height;
	result.shm_info.shmid = shmget(IPC_PRIVATE, total_size, IPC_CREAT|0777);

	result.shm_info.shmaddr = result.image->data = shmat(result.shm_info.shmid, 0, 0);
	result.shm_info.readOnly = False;

	Status status = XShmAttach(result.display, &result.shm_info);
	XSync(result.display, False);
	shmctl(result.shm_info.shmid, IPC_RMID, 0);

	Assert(status);

# else
	printf("[MIT-SHM unsupported by X11 Falling back]\n");

	result.image_buffer = ArenaPushArray(arena, U8, sizeof(ColorRGBA) * win_rect.width * win_rect.height);
	result.image = XCreateImage(result.display, 
				DefaultVisualOfScreen(result.screen),
				DefaultDepthOfScreen(result.screen), 
				ZPixmap, 
				0, 
				(char*) result.image_buffer, 
				win_rect.width, win_rect.height, 
				BitsFromBytes(sizeof(ColorRGBA)), 0); 

	Assert(result.image);

#endif
	result.graphics_ctx = XCreateGC(result.display, result.window, 0, &result.graphics_ctx_values);

	XStoreName(result.display, result.window, str8_to_cstring(arena, window_name));

	XMapWindow(result.display, result.window);
	return result;	
}

void wm_resize_window(WM_Context* ctx, U16 width, U16 height){
	XResizeWindow(ctx->display, ctx->window, width, height);
}

void wm_move_window(WM_Context* ctx, U16 x, U16 y){
	XMoveWindow(ctx->display, ctx->window, x, y);
}


void wm_resize_and_move_window(WM_Context* ctx, RectU16 new_size){
	U32 change_mask = WM_WindowCfgUpdate_X | WM_WindowCfgUpdate_Y | WM_WindowCfgUpdate_Width | WM_WindowCfgUpdate_Height;
	XWindowChanges changes = {
		.x = new_size.x,
		.y = new_size.y,
		.width = new_size.width,
		.height = new_size.height
	};
	XConfigureWindow(ctx->display, ctx->window, change_mask, &changes);
}


void wm_draw_window(WM_Context* ctx){
	Assert(ctx);
	Assert(ctx->display);
	Assert(ctx->window);
	Assert(ctx->image);

#if HAS_SYS_SHM	
	XShmPutImage(ctx->display, ctx->window, ctx->graphics_ctx, ctx->image, 0, 0, 0, 0, ctx->size.width, ctx->size.height, False);
	XSync(ctx->display, False);
#else
	XPutImage(ctx->display, ctx->window, ctx->graphics_ctx, ctx->image, 0, 0, 0, 0, ctx->size.width, ctx->size.height);
	XFlush(ctx->display);
#endif
}

// 2d primitive drawing
void wm_draw_rect(WM_Context* ctx, RectF32 rect, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	
	U32 x1 = Max(0, rect.x);
	U32 y1 = Max(0, rect.y);
	U32 x2 = Min(ctx->size.width, (rect.x + rect.width));
	U32 y2 = Min(ctx->size.height, (rect.y + rect.height));

	if( x1 >= x2 || y1 >= y2 ) return;

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	U32 width = x2 - x1;
	U32 color_const = color_rgba_to_bgra(color).c;

	// this points to the first pixel in the rect
	U32* row_ptr = pixels + (y1 * stride) + x1;

	for (U32 y = y1; y < y2; y++){
		for (U32 x = 0; x < width; x++){
			row_ptr[x] = color_const; 
		}
		row_ptr += stride;
	}
}


// This method uses Midpoint circle algorithm for quickly drawing a circle
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm 
void wm_draw_circle(WM_Context* ctx, Vec2F32 center, F32 radius, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);
	Assert(radius > 0.0);

	S32 x0 = (S32) floorf(center.x);
	S32 y0 = (S32) floorf(center.y);
	S32 r  = (S32) radius;

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	S32 window_width = (S32)ctx->size.width;
	S32 window_height = (S32)ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	S32 x = r;
	S32 y = 0;
	S32 decision = 1 - r;

	#define LOCAL_MACRO_DRAWSPAN(py, x_start, x_end) do{\
			if((py) >= (0) && (py) <= (window_height)) {\
      				S32 left = Max(0, (x_start));\
				S32 right = Min((window_width-1),(x_end));\
				for(S32 px = left; px < right; px ++){\
					pixels[(py) * stride + px] = color_const;\
				}\
			}\
		}while(0)

	while(x >= y){
		LOCAL_MACRO_DRAWSPAN(y0+y, x0-x, x0+x);
		LOCAL_MACRO_DRAWSPAN(y0-y, x0-x, x0+x);
		LOCAL_MACRO_DRAWSPAN(y0+x, x0-y, x0+y);
		LOCAL_MACRO_DRAWSPAN(y0-x, x0-y, x0+y);

		y++;
		if(decision <= 0){
			decision += 2 * y + 1;
		}else{
			x--;
			decision += 2 * (y - x) + 1;
		}
	}
	#undef Local_Macro_DRAWSPAN
} 

// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void wm_draw_line(WM_Context* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	S32 window_width = ctx->size.width;
	S32 window_height = ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	S32 x0 = (S32) floorf(p1.x);
	S32 y0 = (S32) floorf(p1.y); 
	S32 x1 = (S32) floorf(p2.x); 
	S32 y1 = (S32) floorf(p2.y);


	S32 dx = abs(x1 - x0);
	S32 dy = -abs(y1 - y0);
	S32 sx = (x0 < x1)? 1 : -1;
	S32 sy = (y0 < y1)? 1 : -1;
	S32 err = dx+dy;
	S32 err_2;

	for(;;){
		if( 0 <= x0 && x0 < window_width && 0 <= y0 && y0 < window_height){
			pixels[y0 * stride + x0] = color_const;
		}
		
		err_2 = 2 * err;

		if(err_2 >= dy){
			if(x0 == x1) break;
			err += dy;
			x0 += sx;
		}
		if(err_2 <= dx){
			if(y0 == y1) break;
			err += dx;
			y0 += sy;
		}
	}
}

void wm_draw_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	wm_draw_line(ctx, v1, v2, color);
	wm_draw_line(ctx, v2, v3, color);
	wm_draw_line(ctx, v3, v1, color);
} 

B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3){
	B32 result = false;	
		 		
	return result;	
}

void wm_draw_filled_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	U32 window_width = ctx->size.width;
	U32 window_height = ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	// get bounding box
	U32 x_min = Min(0, Min(v1.x, Min(v2.x, v3.x)));
	U32 y_min = Min(0, Min(v1.y, Min(v2.y, v3.y)));
	U32 x_max = Min(window_width, Max(v1.x, Max(v2.x, v3.x)));
	U32 y_max = Min(window_height, Max(v1.y, Max(v2.y, v3.y)));

	Rng1U32 x_rng = Rng1_U32(x_min, x_max);
	Rng1U32 y_rng = Rng1_U32(y_min, y_max);

	for EachInRange(y, y_rng){
		for EachInRange(x, x_rng){
			if(wm_is_point_in_triangle(Vec2_F32(x, y), v1, v2, v3)){
				pixels[y * stride + x] = color_const;
			}
		}	
	}
}


void wm_close_window(WM_Context* ctx){
	XUnmapWindow(ctx->display, ctx->window);
	XDestroyWindow(ctx->display, ctx->window);
	XFreeGC(ctx->display, ctx->graphics_ctx);
	XCloseDisplay(ctx->display);
}

void wm_register_input_events(WM_Context* ctx, WM_EventFlag flags){
	long event_mask = 0;	

	for EachIndex(i, ArrayCount(wm_event_mask_map)){
		if(flags & wm_event_mask_map[i].flag){
			event_mask |= wm_event_mask_map[i].event_mask;
		}
	}
	
	XSelectInput(ctx->display, ctx->window, event_mask);
}

U32 wm_num_of_pending_events(WM_Context* ctx){
	return XPending(ctx->display);
}


#endif // BASE_ENABLE_WINDOW

#endif // BASE_IMPLEMENTATION

#ifndef BASE_LOG_H
#define BASE_LOG_H

///////////////////////////////////////
/// cjk: Logging 

typedef U64 LOG_Level;
enum{
	LOG_Level_All = 0,
	LOG_Level_Debug,
	LOG_Level_Info,
	LOG_Level_Warning,
	LOG_Level_Error,
	LOG_Level_Fatal,
	LOG_Level_None,
	LOG_Level_Count, // dont use this its just good for looping 
} ;

typedef U64 LOG_Option;
enum{
	LOG_Option_None		= 0,
	LOG_Option_TimeStamp	= (1 << 0),
	LOG_Option_Abort	= (1 << 1),
	LOG_Option_ForceFlush 	= (1 << 2),
	LOG_Option_MetaData 	= (1 << 3),
	LOG_Option_Category	= (1 << 4)
};


typedef struct{
	U64 fd;
	LOG_Level level;	
	LOG_Option log_flags;
	Str8	category;
} LOG_Context;

read_only const char* log_level_strings[] = {
	"All", // unused
	"DEBG",
	"INFO",
	"WARN",
	"ERRO",
	"FATL"
};

read_only const char* log_level_color_codes[] = {
	"",
	"",
	"",
	"\033[1;33m", // yellow
	"\033[1;31m", // red
	"\033[1;31m"  // red
};

#if COMPILER_GCC || COMPILER_CLANG
# define LOG_FORMAT_CHECK(fmt, args) __attribute__((format(printf, fmt, args)))
# if __has_builtin(__builtin_COLUMN)
#  define __LOG_COL__ __builtin_COLUMN()
# else
#  define __LOG_COL__ 0
# endif
#else
# define LOG_FORMAT_CHECK(fmt, args)
# define __LOG_COL__ 0
#endif

#define LOG_MAX_LENGTH 512 

// these use the defualt log context to std err and allow all types of logs
#define LogDebug(...) 		log_msg((const LOG_Context){2, 0, 0, {0}}, LOG_Level_Debug, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogInfo(...) 		log_msg((const LOG_Context){2, 0, 0, {0}}, LOG_Level_Info, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogWarning(...) 	log_msg((const LOG_Context){2, 0, 0, {0}}, LOG_Level_Warning, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogError(...) 		log_msg((const LOG_Context){2, 0, 0, {0}}, LOG_Level_Error, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogFatal(...) 		log_msg((const LOG_Context){2, 0, 0, {0}}, LOG_Level_Fatal, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)

// to define a log context pass them into this function
#define LogCtxDebug(ctx, ...)	log_msg((ctx), LOG_Level_Debug, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogCtxInfo(ctx, ...)    log_msg((ctx), LOG_Level_Info, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogCtxWarning(ctx, ...) log_msg((ctx), LOG_Level_Warning, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogCtxError(ctx, ...)   log_msg((ctx), LOG_Level_Error, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)
#define LogCtxFatal(ctx, ...)   log_msg((ctx), LOG_Level_Fatal, __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)

// to get special logs create and pass in a log context and a level 
#define LogMsg(log_ctx, level, ...) 	log_msg((log_ctx), (level), __FILE__, __LINE__, __LOG_COL__, __FUNCTION__, __VA_ARGS__)

void log_msg(LOG_Context ctx, LOG_Level lvl, const char* file, U64 line, U64 col, const char* function, const char* fmt,  ...) LOG_FORMAT_CHECK(7, 8);

const U8* log_filename_from_path(const U8* path);

#endif // BASE_LOG_H

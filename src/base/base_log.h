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
	LOG_Level_Count,
};

const char* log_level_strings[] = {
	"[All]",
	"[Debug]",
	"[Info]",
	"[Warning]",
	"[Error]",
	"[Fatal]"
};

const char* log_level_color_codes[] = {
	"",
	"",
	"",
	"\033[1;33m", // yellow
	"\033[1;31m", // red
	"\033[1;31m"  // red
};

#define LOG_MAX_LENGTH 512 

#define LogDebug(...) log_msg(LOG_Level_Debug, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LogInfo(...) log_msg(LOG_Level_Info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LogWarning(...) log_msg(LOG_Level_Warning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LogError(...) log_msg(LOG_Level_Error, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LogFatal(...) log_msg(LOG_Level_Fatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#if COMPILER_GCC || COMPILER_CLANG
# define LOG_FORMAT_CHECK(fmt, args) __attribute__((format(printf, fmt, args)))
#else
# define LOG_FORMAT_CHECK(fmt, args)
#endif

global LOG_Level glb_log_level = LOG_Level_All;

void log_set_current_glb_level(LOG_Level type);

void log_msg(LOG_Level lvl, const char* file, U64 line, const char* function, const char* fmt,  ...) LOG_FORMAT_CHECK(5, 6);

const char* log_filename_from_path(const char* path);

#endif // BASE_LOG_H

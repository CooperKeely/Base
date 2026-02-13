#ifndef BASE_OS_H
#define BASE_OS_H
///////////////////////////////////////
/// cjk: OS API Definitions

# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/sysinfo.h>
# include <execinfo.h>
# include <signal.h>
# include <linux/limits.h>

typedef S32 OS_ApplicationEntryPoint(U64 argc, U8** argv);

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
	Str8 binary_path;
	Str8 initial_path;
	Str8 user_program_data_path;
} OS_ProcessInfo;

typedef struct {
	OS_FileIterFlags flags;
	U8 buf[PATH_MAX];
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

typedef struct{
	OS_SystemInfo sys_info;
	OS_ProcessInfo proc_info;
	Arena* arena;
} OS_State;


// TODO: Finish os implementation
//	1) OS entry point - DONE
//	2) Basic File operations - DONE
//	3) System and process info - DONE
//	4) OS time info - DONE
//	5) Memory Allocation
//	6) Directory iteration and creation
//	7) file memory maps
//	8) Large page allocations
//	9) DLL support

global OS_State os_state = {0};

// OS entry point
void os_entry_point(U64 argc, U8 **argv, OS_ApplicationEntryPoint* app);

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
OS_FileIter *os_file_iter_begin(Arena *arena, Str8 path, OS_FileIterFlags flags);
B32 os_file_iter_next(Arena *arena, OS_FileIter *iter, OS_FileInfo *info_out);
void os_file_iter_end(OS_FileIter *iter);

// Directory creation
B32 os_make_directory(Str8 path);

// System and process info
Str8 os_get_current_path(Arena* arena);
OS_SystemInfo os_get_system_info();

// OS memory allocation
void *os_reserve_memory(U64 size);
void os_commit_memory(void *ptr, U64 size);
void os_decommit_memory(void *ptr, U64 size);
void os_release_memory(void *ptr, U64 size);

// OS time
U64 os_now_microseconds();
DateTime os_now_universal_time();
DateTime os_now_local_time();
DateTime os_universal_time_from_local(DateTime local_time);
DateTime os_local_time_from_universal(DateTime universal_time);
void os_sleep_milliseconds(U64 msec);

#endif // BASE_OS_H

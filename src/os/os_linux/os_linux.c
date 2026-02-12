
#include "os_linux.h"

///////////////////////////////////////
/// cjk: Linux API Helper Functions 

DateTime os_lnx_datetime_from_tm(struct tm tm_time){
	DateTime result = (DateTime){
		.sec = tm_time.tm_sec,
		.min = tm_time.tm_min,
		.hour = tm_time.tm_hour,
		.day = tm_time.tm_mday,
		.month_num = tm_time.tm_mon,
		.year = tm_time.tm_year + 1900,
		.week_day = tm_time.tm_wday,
	};
	return result;
}

	
struct tm os_lnx_tm_from_datetime(DateTime time){
	struct tm tm_time = (struct tm){
		.tm_sec = time.sec,
		.tm_min = time.min,
		.tm_hour = time.hour,
		.tm_mday = time.day,
		.tm_mon = time.month_num,
		.tm_year = time.year - 1900,
		.tm_isdst = -1,
	};
	return tm_time;
}

DateTime os_lnx_datetime_from_unixtime(U64 unix_time) {
	DateTime date 	= {0};
	date.year 	= 1970;
	date.day 	= 1 + (unix_time / 86400);
	date.min 	= (U32)(unix_time / 60) % 60;
	date.sec 	= (U32)unix_time % 60;
	date.hour 	= (U32)(unix_time / 3600) % 24;

	local_persist const U32 month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	for(;;){
		Bool is_leap = ((date.year % 4 == 0) && ((date.year % 100) != 0 || (date.year % 400) == 0));

		for (date.month = 0; date.month < 12; ++date.month) {
			U64 c = month_days[date.month];
			if(date.month == Month_Feb && is_leap) c = 29;	

			if (date.day <= c) {
				date.month++; // this is needed to be from 1-12
				goto exit;
			}
			date.day -= c;
		}
		++date.year;
	}
	exit:;
	return date;
}

time_t os_lnx_time_from_datetime(DateTime time){
	time_t lnx_time = {0};
	U64 seconds_in_a_day = 24 * 60 * 60;

	local_persist const U32 month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	for(U32 year = 1970; year < time.year; year ++){
		Bool is_leap = false;
		if ((year % 4 == 0) && ((year % 100) != 0 || (year % 400) == 0)) is_leap = true;
		lnx_time += (is_leap ? 366 : 365) * seconds_in_a_day;
	}

	Bool is_leap = ((time.year % 4 == 0) && ((time.year % 100) != 0 || (time.year % 400) == 0));
	for (U32 month = 0; month < time.month_num; month++) {
		U64 c = 0;
		c = month_days[month];

		if(month == Month_Feb && is_leap) c = 29;

		lnx_time += c * seconds_in_a_day;
	}
	
	lnx_time += (time.day - 1) * seconds_in_a_day;
	lnx_time += time.hour * 60 * 60;
	lnx_time += time.min * 60;
	lnx_time += time.sec;

	return lnx_time;
}

DateTime os_lnx_datetime_from_timespec(struct timespec time){
	struct tm tm_time;
	void* err = localtime_r(&time.tv_sec, &tm_time);
	Assert(err != NULL);

	DateTime result = os_lnx_datetime_from_tm(tm_time);
	result.micro_sec = (time.tv_nsec / Thousand(1));
	result.mil_sec = (time.tv_nsec / Million(1));

	return result;
}


void os_lnx_signal_handler(int sig, siginfo_t *info, void *arg) {
	local_persist void *bt_buffer[PATH_MAX];
	U64 bt_count = backtrace(bt_buffer, ArrayCount(bt_buffer));

	fprintf(stderr, "\n");

	ThreadCTX* tctx = thread_ctx_get_selected();
	Str8 thread_str = str8(tctx->thread_name, tctx->thread_name_size);
	fprintf(stderr, "[Thread %.*s %s:%lu recieved signal: %s (%d)]\n", Str8VArg(thread_str), tctx->file_name, tctx->line_number, strsignal(sig), sig);

	if(errno != 0){
		int err = errno;
		const char* error_description = strerrordesc_np(err);
		const char* error_name = strerrorname_np(err);
		fprintf(stderr, "[Errno (%d) %s: %s]\n", err, error_name, error_description);
	}

	for EachIndex(i, bt_count) {

		Dl_info dl_info = {0};
		dladdr(bt_buffer[i], &dl_info);

		char cmd[PATH_MAX];
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
		} 

		else {
			fprintf(stderr, "%ld. [0x%016lx] %s\n", i + 1,(unsigned long)bt_buffer[i], dl_info.dli_fname);
		}
	}
	exit(0);
}



///////////////////////////////////////
/// cjk: OS Linux API Functions 

void os_entry_point(U64 argc, U8 **argv, OS_ApplicationEntryPoint* app) {

	// signal handeler setup
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

	// setup satically allocated system and proc info
	os_state.sys_info.logical_processor_count = get_nprocs();
	os_state.sys_info.page_size = getpagesize();
	os_state.sys_info.large_page_size = MB(2);
	os_state.sys_info.allocation_ganularity = os_state.sys_info.page_size;

	// process info setup
	os_state.proc_info.pid = getpid();

	// allocate os state arena	
	os_state.arena = arena_alloc();
	
	// thread context setup
	ThreadCTX* tctx = ThreadCTXAlloc("main_thread");
	thread_ctx_select(tctx);	

	// dynamically allocated system info
	ScratchArenaScope(scratch, 0, 0){
		OS_SystemInfo* sys_info = &os_state.sys_info;
		char* char_buf = ArenaPushArray(scratch.arena, char, PATH_MAX);
		S32 err = gethostname(char_buf, PATH_MAX);
		Assert(err != -1);
		
		Str8 host_name = cstring_to_str8(char_buf); 
		
		sys_info->machine_name.size = host_name.size;
		sys_info->machine_name.str = ArenaPushArray(os_state.arena, U8, host_name.size);
		MemoryCopyStr8(sys_info->machine_name, host_name);	
	}
	
	// dynamically allocated process info	
	ScratchArenaScope(scratch, 0, 0){
		OS_ProcessInfo* proc_info = &os_state.proc_info;
		proc_info->initial_path = os_get_current_path(os_state.arena);

		char* home = getenv("HOME");	
		Assert(home);

		Str8 home_str8 = cstring_to_str8(home);
		proc_info->user_program_data_path = str8_copy(os_state.arena, home_str8);

		char* char_buff = ArenaPushArray(scratch.arena, char, PATH_MAX);
		S32 err = readlink("/proc/self/exe", char_buff, PATH_MAX);
		Assert(err != -1);

		Str8 binary_path = str8((U8*) char_buff,(U64) err);
		proc_info->binary_path = str8_copy(os_state.arena, binary_path);
	}


#   ifdef BUILD_DEBUG
	OS_ProcessInfo* proc_info = &os_state.proc_info;
	OS_SystemInfo* info = &os_state.sys_info;

	str8_printf(stderr, "[Process Debug Info]\n");
	str8_printf(stderr, "[Machine name: %.*s]\n", Str8VArg(info->machine_name));
	str8_printf(stderr, "[Page Size: %lu]\n", info->page_size);
	str8_printf(stderr, "[Large Page Size: %lu]\n", info->large_page_size);
	str8_printf(stderr, "[CPU Cores: %d]\n", info->logical_processor_count);
	str8_printf(stderr, "[PID: %d]\n", proc_info->pid);
	str8_printf(stderr, "[Home Path: %.*s]\n", Str8VArg(proc_info->user_program_data_path));
	str8_printf(stderr, "[Inital Path: %.*s]\n", Str8VArg(proc_info->initial_path));
	str8_printf(stderr, "[Binary Path: %.*s]\n", Str8VArg(proc_info->binary_path));
	str8_printf(stderr, "\n"); 
#   endif

	// call user entry point
	S32 exit_code = app(argc, argv);
	exit(exit_code);
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

	ScratchArenaScope(scratch, 0, 0){
		S32 err = fstat(file_handle, &statbuf);
		Assert(err != -1);	

		// time modified and created	
		DateTime date_created = os_lnx_datetime_from_timespec(statbuf.st_ctim);
		DateTime last_modified = os_lnx_datetime_from_timespec(statbuf.st_mtim);
		props.created = densetime_from_datetime(date_created);
		props.modified = densetime_from_datetime(last_modified);

		// file properties
		if(S_ISREG(statbuf.st_mode)){
			props.flags = OS_FilePropertyFlag_IsFile;
		}else if(S_ISDIR(statbuf.st_mode)){
			props.flags = OS_FilePropertyFlag_IsFolder;
		}
		
		// file name
		Str8 path = Str8Lit("/proc/self/fd/");
		Str8 fd = integer_to_str8(scratch.arena, file_handle);
		Str8 fd_path = str8_concat(scratch.arena, path, fd);
		const char* cstr_fd_path = str8_to_cstring(scratch.arena, fd_path);
		
		char* char_buf = ArenaPushArray(scratch.arena, char, PATH_MAX);
		S32 bytes_read = readlink(cstr_fd_path, char_buf, PATH_MAX);
		Assert(bytes_read != -1);	

		Str8 file_name = str8((U8*) char_buf, bytes_read);
		props.name = str8_copy(scratch.arena, file_name);
	}

	return props;
}

OS_FileProperties os_properties_from_file_path(Str8 path) {
	OS_FileProperties props = {0};
	struct stat statbuf;

	ScratchArenaScope(scratch, 0, 0){
		char* cstr_file_path = str8_to_cstring(scratch.arena, path);

		S32 err = stat(cstr_file_path, &statbuf);
		Assert(err != -1);	

		// time modified and created
		DateTime date_created = os_lnx_datetime_from_timespec(statbuf.st_ctim);
		DateTime last_modified = os_lnx_datetime_from_timespec(statbuf.st_mtim);
		props.created = densetime_from_datetime(date_created);
		props.modified = densetime_from_datetime(last_modified);
		
		// file properties
		if(S_ISREG(statbuf.st_mode)){
			props.flags = OS_FilePropertyFlag_IsFile;
		}else if(S_ISDIR(statbuf.st_mode)){
			props.flags = OS_FilePropertyFlag_IsFolder;
		}
		
		// get file name
		Str8 file_path = str8_skip_last_slash(path);
		props.name = str8_copy(scratch.arena, file_path);
	}

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
Str8 os_get_current_path(Arena* arena) { 
	void* char_buf = getcwd(0, 0);
	Str8 curr_dir = cstring_to_str8(char_buf);
	
	Str8 result = str8_copy(arena, curr_dir);

	free(char_buf);
	return result;	
}


OS_SystemInfo os_get_system_info() { 
	return os_state.sys_info;
}

// OS memory allocation
void *os_reserve_memory(U64 size) { NotImplemented; }
void os_commit_memory(void *ptr, U64 size) { NotImplemented; }
void os_decommit_memory(void *ptr, U64 size) { NotImplemented; }
void os_release_memory(void *ptr, U64 size) { NotImplemented; }

// OS time
U64 os_now_microseconds() {
	struct timespec time_spec;
	S32 err = clock_gettime(CLOCK_REALTIME, &time_spec);

	return time_spec.tv_nsec/ Thousand(1);
}

DateTime os_now_universal_time() {
	DateTime now_UTC;
	struct tm tm_time;
	time_t current_time;

	time(&current_time);
	void* err = gmtime_r(&current_time, &tm_time);
	Assert(err);

	now_UTC = os_lnx_datetime_from_tm(tm_time);
	return now_UTC;
}

DateTime os_now_local_time(){
	struct tm tm_time;
	time_t current_time;

	time(&current_time);
	void* err = localtime_r(&current_time, &tm_time);
	Assert(err);

	DateTime now_local_time = os_lnx_datetime_from_tm(tm_time);
	return now_local_time;
}

DateTime os_universal_time_from_local(DateTime local_time) {
	struct tm local_tm = os_lnx_tm_from_datetime(local_time); 

	time_t utc_time = mktime(&local_tm);

	struct tm tm_utc;
	void* err = gmtime_r(&utc_time, &tm_utc);
	Assert(err);

	return os_lnx_datetime_from_tm(tm_utc);
}

DateTime os_local_time_from_universal(DateTime universal_time) {
	time_t time = os_lnx_time_from_datetime(universal_time);
	
	struct tm tm_local;
	void* err  = localtime_r(&time, &tm_local);
	Assert(err);

	return os_lnx_datetime_from_tm(tm_local);
}

void os_sleep_milliseconds(U64 msec) {
	U64 nano_sec = msec * Million(1);
	
	struct timespec sleep_time;
	sleep_time.tv_nsec = nano_sec;
	struct timespec remaining;
	S32 err = clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep_time, &remaining);
	Assert(err == 0);
}


void log_msg(LOG_Context ctx, LOG_Level lvl, const char* file, U64 line, U64 col, const char* function, const char* fmt,  ...) {
	if(lvl < ctx.level) return;

	char log_buf[LOG_MAX_LENGTH]; // 512 bytes
	U64 offset = 0;
	const U8* clean_file = log_filename_from_path((U8*) file);
	
	// set color
	offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "%s", log_level_color_codes[lvl]);
	
	// print time stamp if needed
	if(FLAG_IS_SET(ctx.log_flags, LOG_Option_TimeStamp)){
		DateTime curr_time = os_now_local_time();	
		offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "[%04d/%02d/%02d %02d:%02d:%02d] ", 
		     			curr_time.year, curr_time.month + 1, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec); 
	}

	// set color and static string
	offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "[%s] ", log_level_strings[lvl]);

	// print the category if option is set
	if(FLAG_IS_SET(ctx.log_flags, LOG_Option_Category)){
		offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "[%.*s] ", Str8VArg(ctx.category));
	}
	
	// print the users log
	va_list args;
	va_start(args, fmt);
	offset += vsnprintf(log_buf + offset, LOG_MAX_LENGTH - offset, fmt, args);
	va_end(args);	
	
	// print file function and line meta data
	if(FLAG_IS_SET(ctx.log_flags, LOG_Option_MetaData)){
		offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "\033[90m"); // dark grey
		offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, " | %s:%lu:%lu (%s)", clean_file, line, col, function);
	}
	
	offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "\033[0m\n"); // reset color and new line

	dprintf(ctx.fd, "%s", log_buf);
	
	if(FLAG_IS_SET(ctx.log_flags, LOG_Option_ForceFlush)){
		fsync(ctx.fd);
	}

	if(FLAG_IS_SET(ctx.log_flags, LOG_Option_Abort)){
		if(lvl >= LOG_Level_Error){
			Assert(BASE_FALSE);
		}	
	}
}

const U8* log_filename_from_path(const U8* path){
	const U8* last_slash = path;
	for(const U8* p = path; *p; p++){
		if(*p == '\\' || *p == '/' ) last_slash = p + 1;
	}
	return last_slash;
}



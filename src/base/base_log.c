
void log_set_current_glb_level(LOG_Level type){
	glb_log_config.current_level = type;		
}

void log_set_current_glb_fd(uint64_t fd){
	glb_log_config.fd = fd;		
}


void log_msg(LOG_Level lvl, const char* file, int line, int col, const char* function, const char* fmt,  ...) {
	if(lvl < glb_log_config.current_level) return;
	if(fmt == NULL) return;

	char log_buf[LOG_MAX_LENGTH]; // 512 bytes
	U64 offset = 0;
	const char* clean_file = log_filename_from_path(file);
	
	// set color and static string
	offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "%s%-9s ", log_level_color_codes[lvl], log_level_strings[lvl]);
	
	// print the users log
	va_list args;
	va_start(args, fmt);
	offset += vsnprintf(log_buf + offset, LOG_MAX_LENGTH - offset, fmt, args);
	va_end(args);	
	
	// print file function and line
	offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "\033[90m"); // set the metadata to a dark grey color
	offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, " | %s:%d:%d (%s)", clean_file, line, col, function);
	
	// reset color and print new line
	offset += snprintf(log_buf + offset, LOG_MAX_LENGTH - offset, "\033[0m\n");

	fprintf(stderr, "%s", log_buf);
}

const char* log_filename_from_path(const char* path){
	const char* last_slash = path;
	for(const char* p = path; *p; p++){
		if(*p == '\\' || *p == '/' ) last_slash = p + 1;
	}
	return last_slash;
}



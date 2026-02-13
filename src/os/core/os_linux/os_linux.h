
///////////////////////////////////////
/// cjk: Linux API Helper Functions 

DateTime os_lnx_datetime_from_tm(struct tm tm_time);

	
struct tm os_lnx_tm_from_datetime(DateTime time);

DateTime os_lnx_datetime_from_unixtime(U64 unix_time);

time_t os_lnx_time_from_datetime(DateTime time);

DateTime os_lnx_datetime_from_timespec(struct timespec time);


void os_lnx_signal_handler(int sig, siginfo_t *info, void *arg);


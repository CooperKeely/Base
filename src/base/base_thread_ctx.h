

///////////////////////////////////////
/// cjk: Thread Local Arena Definition

typedef struct {
	U8 	thread_name[32];
	U64 	thread_name_size;
	Arena* 	arenas[2];
	char* 	file_name;
	U64 	line_number;
} ThreadCTX;

#define ThreadCTXAlloc(thread_name) 		thread_ctx_alloc(__FILE__, __LINE__, (thread_name))
#define GetScratchArena(conflicts, count) 	temp_arena_begin(thread_ctx_get_scratch((conflicts), (count)))
#define EndScratchArena(scratch) 		temp_arena_end((scratch))

#define ScratchArenaScope(temp_name, conflicts, count) TempArenaScope(temp_name, thread_ctx_get_scratch((conflicts), (count)))

thread_static ThreadCTX* local_thread_ctx = NULL;

ThreadCTX* thread_ctx_alloc(char* file_name, U64 line_number, char* thread_name);
void thread_ctx_release(void);
void thread_ctx_select(ThreadCTX* thread_ctx);
ThreadCTX* thread_ctx_get_selected(void);

Arena* thread_ctx_get_scratch(Arena** conflicts, U64 count);



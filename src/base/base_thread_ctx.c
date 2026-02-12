
#include "base_thread_ctx.h"

///////////////////////////////////////
/// cjk: Thread Local Arena Definition

ThreadCTX* thread_ctx_alloc(char* file_name, U64 line_number, char* thread_name){
	Arena* arena = arena_alloc();
	ThreadCTX* tctx = ArenaPushStructZero(arena, ThreadCTX);
	tctx->arenas[0] = arena;
	tctx->arenas[1] = arena_alloc();
	tctx->file_name = file_name;
	tctx->line_number = line_number;
	tctx->thread_name_size  = ClampTop(cstring_length(thread_name), 32);
	MemoryCopy(tctx->thread_name, thread_name, tctx->thread_name_size);
	return tctx;
}

void thread_ctx_release(void){
	arena_release(local_thread_ctx->arenas[0]);
	arena_release(local_thread_ctx->arenas[1]);
}

void thread_ctx_select(ThreadCTX* thread_ctx){
	local_thread_ctx = thread_ctx;
}

ThreadCTX* thread_ctx_get_selected(void){
	return local_thread_ctx;
}


Arena* thread_ctx_get_scratch(Arena** conflicts, U64 count){
	ThreadCTX* selected_ctx = thread_ctx_get_selected();
	Arena* result = 0;
	Arena** arena_ptr = selected_ctx->arenas;
	for(U64 i = 0; i < ArrayCount(selected_ctx->arenas); i += 1, arena_ptr += 1){
		Arena** conflict_ptr = conflicts;	
		B32 has_conflict = false;
		for(U64 j = 0; j < count; j += 1, conflict_ptr += 1){
			if(*arena_ptr == *conflict_ptr){
				has_conflict = true;
				break;
			}
		}
		if(!has_conflict){
			result = *arena_ptr;
			break;
		}
	}

	Assert(result);
	return result;
}

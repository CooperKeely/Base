

typedef struct OS_StreamChunk OS_StreamChunk;
struct OS_StreamChunk{
	Buffer contents;
	OS_StreamChunk* next;
};

typedef struct OS_Stream OS_Stream;
struct OS_Stream{
	Arena* arena;
	OS_Stream* errors;	

	Buffer contents;

	U32 bit_buffer;
	U32 bit_count;
	B32 underflow;

	OS_StreamChunk* first;
	OS_StreamChunk* last;
};



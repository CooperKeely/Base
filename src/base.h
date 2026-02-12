#ifndef BASE_H
#define BASE_H

///////////////////////////////////////
/// cjk: PRNG Definitions 
/// https://en.wikipedia.org/wiki/Mersenne_Twister

// TODO: (cjk) implement Mersenne Twiser psuedo random number generation


///////////////////////////////////////
/// cjk: Color Definitions 

typedef union{
	struct{
		U8 r;
		U8 g;
		U8 b;
		U8 a;
	};
	U32 c;
}ColorRGBA;

typedef union{
	struct{
		U8 b;
		U8 g;
		U8 r;
		U8 a;
	};
	U32 c;
}ColorBGRA;

#define RGB(R,G,B)	color_rgba((U8)(R), (U8)(G), (U8)(B), (U8)255) 
#define RGBA(R,G,B,A)	color_rgba((U8)(R), (U8)(G), (U8)(B), (U8)(A))

// Color pallette
#define COLOR_WHITE 	RGB(255,255,255)
#define COLOR_BLACK 	RGB(0,0,0)
#define COLOR_RED   	RGB(255,0,0) 
#define COLOR_GREEN	RGB(0,255,0) 
#define COLOR_BLUE 	RGB(0, 0, 255) 
#define COLOR_YELLOW 	RGB(255,255,0)
#define COLOR_MAGENTA	RGB(255,0,255) 
#define COLOR_CYAN	RGB(0,255,255) 

ColorRGBA color_rgba(U8 r, U8 g, U8 b, U8 a);
ColorBGRA color_rgba_to_bgra(ColorRGBA color);
///////////////////////////////////////
/// cjk: Hashing Definitions

#define DEFAULT_HASH_MAP_SIZE KB(4)
#define DEFAULT_HASH_COLLISION_SIZE KB(1)

typedef struct {
	void *data;
	Str8 key;
	U64 next_data;
	B32 has_next_data;
} HashBucket;

typedef struct {
	Arena *arena;
	U64 next_free_collision_bucket;
	HashBucket data[DEFAULT_HASH_MAP_SIZE];
	HashBucket collisions[DEFAULT_HASH_COLLISION_SIZE];
} HashMap;

HashMap *hash_map_init(Arena *arena);
void *hash_map_get(HashMap *hash_map, Str8 key);
void hash_map_put(HashMap *hash_map, Str8 key, void *data);

// Hashing functions
U64 hash_fnv1a_u64(const void *in_ptr, U64 num_bytes);



///////////////////////////////////////
/// cjk: Window API Definitions 

#ifdef BASE_ENABLE_WINDOW

# include <math.h>

# include <xcb/xcb.h>
# include <xcb/shm.h>
# include <sys/ipc.h>
# include <sys/shm.h>

/*
typedef U32 WM_WindowCfgUpdate;
enum{
	WM_WindowCfgUpdate_X		= (1<<0),
	WM_WindowCfgUpdate_Y		= (1<<1),
	WM_WindowCfgUpdate_Width	= (1<<2),
	WM_WindowCfgUpdate_Height	= (1<<3),
	WM_WindowCfgUpdate_BorderWidth	= (1<<4),
	WM_WindowCfgUpdate_Sibiling	= (1<<5),
	WM_WindowCfgUpdate_StackMode	= (1<<6),
};


typedef U64 WM_EventFlag;
enum{
	WM_Event_Keyboard_KeyPress		= (1ULL<<0),
	WM_Event_Keyboard_KeyRelease		= (1ULL<<1),
	WM_Event_Mouse_ButtonPress		= (1ULL<<2),
	WM_Event_Mouse_ButtonRelease		= (1ULL<<3),
	WM_Event_Mouse_MotionNotify		= (1ULL<<4),
	WM_Event_Window_EnterNotify		= (1ULL<<5),
	WM_Event_Window_LeaveNotify		= (1ULL<<6),
	WM_Event_Focus_FocusIn			= (1ULL<<7),
	WM_Event_Focus_FocusOut			= (1ULL<<8),
	WM_Event_KeyMap_KeymapNotify		= (1ULL<<9),
	WM_Event_Exposure_Expose		= (1ULL<<10),
	WM_Event_Exposure_GraphicsExpose	= (1ULL<<11),
	WM_Event_Exposure_NoExpose		= (1ULL<<12),
	WM_Event_Structure_CirculateRequest	= (1ULL<<13),
	WM_Event_Strucutre_ConfigureRequest	= (1ULL<<14),
	WM_Event_Structure_MapRequest		= (1ULL<<15),
	WM_Event_Structure_ResizeRequest	= (1ULL<<16),
	WM_Event_WindowState_CirculateNotify	= (1ULL<<17),
	WM_Event_WindowState_ConfigureNotify	= (1ULL<<18),
	WM_Event_WindowState_CreateNotify	= (1ULL<<19),
	WM_Event_WindowState_DestroyNotify	= (1ULL<<20),
	WM_Event_WindowState_GravityNotif	= (1ULL<<21),
	WM_Event_WindowState_MapNotify		= (1ULL<<22),
	WM_Event_WindowState_MappingNotify	= (1ULL<<23),
	WM_Event_WindowState_ReparentNotify	= (1ULL<<24),
	WM_Event_WindowState_UnmapNotify	= (1ULL<<25),
	WM_Event_WindowState_VisibilityNotify	= (1ULL<<26),
	WM_Event_Colormap_ColormapNotify	= (1ULL<<27),
	WM_Event_ClientComm_ClientMessage	= (1ULL<<28),
	WM_Event_ClientComm_PropertyNotify	= (1ULL<<29),
	WM_Event_ClientComm_SelectionClear	= (1ULL<<30),
	WM_Event_ClientComm_SelectionNotify	= (1ULL<<31),
	WM_Event_ClientComm_SelectionRequest	= (1ULL<<32),
};

// Maps our window manager events to x11 event request
typedef struct{
	WM_EventFlag flag;	
	long event_mask;
}WM_EventMaskMap;

global WM_EventMaskMap wm_event_mask_map[] = {
	{WM_Event_Keyboard_KeyPress		,KeyPressMask},
	{WM_Event_Keyboard_KeyRelease		,KeyReleaseMask},
	{WM_Event_Mouse_ButtonPress		,ButtonPressMask},
	{WM_Event_Mouse_ButtonRelease		,ButtonReleaseMask},
	{WM_Event_Mouse_MotionNotify		,PointerMotionMask},
	{WM_Event_Window_EnterNotify		,EnterWindowMask},
	{WM_Event_Window_LeaveNotify		,LeaveWindowMask},
	{WM_Event_Focus_FocusIn			,FocusChangeMask},
	{WM_Event_Focus_FocusOut		,FocusChangeMask},
	{WM_Event_KeyMap_KeymapNotify		,KeymapStateMask},
	{WM_Event_Exposure_Expose		,ExposureMask},
	{WM_Event_Exposure_GraphicsExpose	,GCGraphicsExposures},
	{WM_Event_Exposure_NoExpose		,GCGraphicsExposures},
	{WM_Event_Structure_CirculateRequest	,SubstructureRedirectMask},
	{WM_Event_Strucutre_ConfigureRequest	,SubstructureRedirectMask},
	{WM_Event_Structure_MapRequest		,SubstructureRedirectMask},
	{WM_Event_Structure_ResizeRequest	,ResizeRedirectMask},
	{WM_Event_WindowState_CirculateNotify	,StructureNotifyMask},
	{WM_Event_WindowState_ConfigureNotify	,StructureNotifyMask},
	{WM_Event_WindowState_CreateNotify	,SubstructureNotifyMask},
	{WM_Event_WindowState_DestroyNotify	,StructureNotifyMask},
	{WM_Event_WindowState_GravityNotif	,StructureNotifyMask},
	{WM_Event_WindowState_MapNotify		,StructureNotifyMask},
	{WM_Event_WindowState_MappingNotify	,NoEventMask},
	{WM_Event_WindowState_ReparentNotify	,StructureNotifyMask},
	{WM_Event_WindowState_UnmapNotify	,StructureNotifyMask},
	{WM_Event_WindowState_VisibilityNotify	,VisibilityChangeMask},
	{WM_Event_Colormap_ColormapNotify	,ColormapChangeMask},
	{WM_Event_ClientComm_ClientMessage	,NoEventMask},
	{WM_Event_ClientComm_PropertyNotify	,PropertyChangeMask},
	{WM_Event_ClientComm_SelectionClear	,NoEventMask},
	{WM_Event_ClientComm_SelectionNotify	,NoEventMask},
	{WM_Event_ClientComm_SelectionRequest	,NoEventMask},
};
*/
typedef struct{
	xcb_connection_t*	connection;
	xcb_screen_t*		screen;
	xcb_window_t		window;	
	xcb_setup_t* 		setup;
	xcb_gcontext_t		graphics_ctx;
	xcb_shm_seg_t		shared_mem_seg;
	U64			shared_mem_seg_id;
	Arena*			arena;
	RectU16			size;
	RectU16			over_size;
	Str8			name;
}WM_Context;


// Open and closing window
WM_Context wm_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, U16 border_width, ColorRGBA border_color,  ColorRGBA background_color);
void wm_close_window(WM_Context* ctx);

// Resizing the window
void wm_resize_window(WM_Context* ctx, U16 width, U16 height);
void wm_move_window(WM_Context* ctx, U16 x, U16 y);
void wm_resize_and_move_window(WM_Context* ctx, RectU16 new_size);
void wm_draw_window(WM_Context* ctx);

// 2d primitive drawing
void wm_draw_rect(WM_Context* ctx, RectF32 rect, ColorRGBA color);
void wm_draw_circle(WM_Context* ctx, Vec2F32 center, F32 radius, ColorRGBA color) ;
void wm_draw_line(WM_Context* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color);
void wm_draw_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color) ;
B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3);
void wm_draw_filled_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color);

// Event handeling
void wm_register_input_events(WM_Context* ctx, WM_EventFlag flags);
U32 wm_num_of_pending_events(WM_Context* ctx);



//////////////////////////////////////
/// cjk: .obj reader Definitions 

// TODO: (cjk) add a .obj file reader to continue on tiny renderer


#endif // BASE_ENABLE_WINDOW

#endif // BASE_H

// Private function definitions
#ifdef BASE_IMPLEMENTATION

///////////////////////////////////////
/// cjk: Base Integer Functions 

U8 safe_cast_U8(U16 x) {
	Assert(x < max_U8);
	return (U8)x;
}

U16 safe_cast_U16(U32 x) {
	Assert(x < max_U16);
	return (U16)x;
}
U32 safe_cast_U32(U64 x) {
	Assert(x < max_U32);
	return (U32)x;
}

S8 safe_cast_S8(S16 x) {
	Assert(x < max_S8);
	Assert(x > min_S8);
	return (S8)x;
}
S16 safe_cast_S16(S32 x) {
	Assert(x < max_S16);
	Assert(x > min_S16);
	return (S16)x;
}
S32 safe_cast_S32(S64 x) {
	Assert(x < max_S32);
	Assert(x > min_S32);
	return (S32)x;
}

///////////////////////////////////////
/// cjk:  Rect Functions 

RectU16 rect_u16(U16 x, U16 y, U16 width, U16 height){ return (RectU16){{x, y, width, height}}; };
RectU32 rect_u32(U32 x, U32 y, U32 width, U32 height){ return (RectU32){{x, y, width, height}}; };
RectU64 rect_u64(U64 x, U64 y, U64 width, U64 height){ return (RectU64){{x, y, width, height}}; };

RectF32 rect_f32(F32 x, F32 y, F32 width, F32 height){ return (RectF32){{x, y, width, height}}; };
RectF64 rect_f64(F64 x, F64 y, F64 width, F64 height){ return (RectF64){{x, y, width, height}}; };


///////////////////////////////////////
/// cjk:  Range Functions 

Rng1U32 rng_1u32(U32 min, U32 max) {
	Assert(min < max);
	return (Rng1U32){{.min = min, .max = max}};
}

Rng1F32 rng_1f32(F32 min, F32 max) {
	Assert(min < max);
	return (Rng1F32){{.min = min, .max = max}};
}

Rng1U64 rng_1u64(U64 min, U64 max) {
	Assert(min < max);
	return (Rng1U64){{.min = min, .max = max}};
}

Rng1F64 rng_1f64(F64 min, F64 max) {
	Assert(min < max);
	return (Rng1F64){{.min = min, .max = max}};
}

U64 dim_r1u64(Rng1U64 r) { 
	return ((r.max > r.min) ? (r.max - r.min) : 0); 
}

///////////////////////////////////////
/// cjk:  Point Functions 

Point2U16 point_2u16(U16 x, U16 y){ return (Point2U16){{x, y}};}
Point2U32 point_2u32(U32 x, U32 y){ return (Point2U32){{x, y}};}
Point2U64 point_2u64(U64 x, U64 y){ return (Point2U64){{x, y}};}

Point3U16 point_3u16(U16 x, U16 y, U16 z){ return (Point3U16){{x, y, z}};}
Point3U32 point_3U32(U32 x, U32 y, U32 z){ return (Point3U32){{x, y, z}};}
Point3U64 point_3U64(U64 x, U64 y, U64 z){ return (Point3U64){{x, y, z}};}

///////////////////////////////////////
/// cjk:  Vector Functions 

// Initializers
Vec2F32 vec_2f32(F32 x, F32 y){ return (Vec2F32){{x, y}};}
Vec2F64 vec_2f64(F64 x, F64 y){ return (Vec2F64){{x, y}};}

Vec3F32 vec_3f32(F32 x, F32 y, F32 z){ return (Vec3F32){{x, y, z}};}
Vec3F64 vec_3f64(F64 x, F64 y, F64 z){ return (Vec3F64){{x, y, z}};}

// dot products
F64 dot_vec2f32(Vec2F32 v1, Vec2F32 v2){ return (v1.x * v2.x) + (v2.y * v2.y);}
F64 dot_vec2f64(Vec2F64 v1, Vec2F64 v2){ return (v1.x * v2.x) + (v2.y * v2.y);}

F64 dot_vec3f32(Vec3F32 v1, Vec3F32 v2){ return (v1.x * v2.x) + (v2.y * v2.y) + (v1.z * v2.z);}
F64 dot_vec3f64(Vec3F64 v1, Vec3F64 v2){ return (v1.x * v2.x) + (v2.y * v2.y) + (v1.z * v2.z);}


///////////////////////////////////////
/// cjk: Color Functions 

ColorRGBA color_rgba(U8 r, U8 g, U8 b, U8 a){ return (ColorRGBA){{r, g, b,a}};}

ColorBGRA color_rgba_to_bgra(ColorRGBA color){
	return (ColorBGRA){
		.b = color.b,
		.g = color.g,
		.r = color.r,
		.a = color.a
	};
}


///////////////////////////////////////
/// cjk: Arena Functions

U64 arena_get_position(Arena *arena) {
	Assert(arena != NULL);
	return arena->next_free;
}

Arena *arena_alloc() { 
	return arena_alloc_with_capacity(DEFAULT_ARENA_SIZE); 
}

Arena *arena_alloc_with_capacity(U64 size) {
	Arena *arena = (Arena *)malloc(sizeof(Arena));
	Assert(arena);

	arena->capacity = size;

	arena->mem_ptr = (U8 *)malloc(arena->capacity);
	Assert(arena->mem_ptr);
	AsanPoisonMemoryRegion(arena->mem_ptr, size);

	arena->next_free = 0;
	return arena;
}

void arena_release(Arena *arena) {
	Assert(arena != NULL);
	arena->capacity = 0;
	arena->next_free = 0;
	free(arena->mem_ptr);
	free(arena);
}

void *arena_push(Arena *arena, U64 size) {
	Assert(arena != NULL);
	if (arena->next_free + size <= arena->capacity) {
		void *ptr = arena->mem_ptr + arena->next_free;
		AsanUnpoisonMemoryRegion(ptr, size);
		arena->next_free += size;
		return ptr;
	}
	InvalidPath;
	return NULL;
}

void *arena_push_zero(Arena *arena, U64 size) {
	Assert(arena != NULL);
	if (arena->next_free + size <= arena->capacity) {
		void *ptr = arena->mem_ptr + arena->next_free;
		AsanUnpoisonMemoryRegion(ptr, size);
		arena->next_free += size;
		memset(ptr, 0, size);
		return ptr;
	}
	InvalidPath;
	return NULL;
}

void arena_pop(Arena *arena, U64 size) {
	Assert(arena != NULL);
	U64 pos = arena->next_free - size;
	arena_pop_to(arena, pos);
}

void arena_pop_to(Arena *arena, U64 new_pos) {
	Assert(arena);
	Assert(arena->mem_ptr);

	U64 current_pos = arena_get_position(arena);

	Assert(new_pos <= current_pos);

	U64 asan_poison_size = current_pos - new_pos;
	void *asan_poison_ptr = arena->mem_ptr + new_pos;
	AsanPoisonMemoryRegion(asan_poison_ptr, asan_poison_size);

	arena->next_free = new_pos;
}

TempArena temp_arena_begin(Arena *arena) {
	Assert(arena != NULL);
	TempArena temp = {arena, arena_get_position(arena)};
	return temp; 
}

void temp_arena_end(TempArena temp) {
	Assert(temp.arena != NULL);
	arena_pop_to(temp.arena, temp.original_position);
}

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


///////////////////////////////////////
/// cjk: String Functions 

U8 upper_from_char(U8 chr) {
	if (chr > 'A') {
		return chr - 32;
	}
	return chr;
}

Str8 str8(U8 *str, U64 length) {
	Assert(str != NULL);
	return (Str8){str, length};
}

void str8_printf(FILE *file_ptr, const char *format, ...) {
	Assert(file_ptr != NULL);
	Assert(format != NULL);
	va_list args;
	va_start(args, format);
	vfprintf(file_ptr, format, args);
	va_end(args);
}

Str8 str8_pushf(Arena *arena, const char *format, ...) {
	Assert(arena);
	Assert(format);

	va_list args;
	va_start(args, format);

	va_list copy;
	va_copy(copy, args);

	U64 len = vsnprintf(NULL, 0, format, copy);

	U8 *ptr = ArenaPushArray(arena, U8, len + 1);
	vsnprintf((char *)ptr, len + 1, format, args);
	arena_pop(arena, 1);

	va_end(args);
	return (Str8){ptr, len};
}

Str8 str8_skip_last_slash(Str8 str) {
	Assert(str.size > 0);
	Assert(str.str);

	U8 *ptr = str.str + str.size - 1;
	for (; ptr >= str.str; ptr -= 1) {
		if (*ptr == '/' || *ptr == '\\') break;
	}

	if (ptr >= str.str) {
		ptr += 1;
		str.size = (U64)(str.str + str.size - ptr);
		str.str = ptr;
	}
	return str;
}

U8 str8_get(Str8 string, U64 idx) {
	if (idx < string.size) {
		return string.str[idx];
	}
	InvalidPath;
	return '\0';
}




Str8 integer_to_str8(Arena* arena, S64 integer){
	if(integer == 0) return Str8Lit("0");
	B32 is_negative = (integer < 0);
	
	U64 abs_val = is_negative ? (U64)(-integer) : (U64)integer;

	U64 digits = 0;
	U64 divide_by = 1;
	for(int i = abs_val; i > 0; i /= 10) {
		digits++; 
		if(i >= 10) divide_by *= 10;
	}

	if(is_negative) digits ++;
	Str8 result = (Str8){
		.str = ArenaPushArray(arena, U8, digits),
		.size = digits 
	};

	if(is_negative) result.str[0] = '-';

	for EachIndex(i, result.size){
		if(is_negative && i == 0) continue;
		U64 digit = (abs_val/divide_by) % 10;
		result.str[i] = (char)(digit + 48);
		divide_by /= 10;
	}	

	return result;
}

U64 cstring_length(const char *c) {
	Assert(c);
	U64 length = 0;
	if (c) {
		U8 *p = (U8 *)c;
		for (; *p != 0; p += 1);
		length = (U64)(p - (U8 *)c);
	}
	return length;
}

Str8 cstring_to_str8(const char *c) {
	Assert(c);
	Str8 ret = {(U8*)c, cstring_length(c)};
	return ret; 
}

char* str8_to_cstring(Arena* arena, Str8 str){
	Assert(arena);
	Assert(str.size > 0);
	Assert(str.str);

	char* cstring = ArenaPushArray(arena, char,str.size + 1);
	MemoryCopy(cstring, str.str, str.size);
	cstring[str.size] = '\0';
	return cstring;	
}

Str8 str8_concat(Arena *arena, Str8 s1, Str8 s2) {
	Assert(arena);

	U64 new_size = s1.size + s2.size;
	U8 *character_buffer = ArenaPushArray(arena, U8, new_size);

	MemoryCopy(character_buffer, s1.str, s1.size);
	MemoryCopy(character_buffer + s1.size, s2.str, s2.size);

	return (Str8){ .str = character_buffer, .size = new_size};
}

Str8 str8_copy(Arena* arena, Str8 str){
	Assert(arena);

	Str8 result = {0};
	result.size = str.size;
	result.str = ArenaPushArray(arena, U8, str.size);
	MemoryCopyStr8(result, str);
	return result;
}

Str8 str8_substr(Str8 s1, Rng1U64 range) {
	range.min = ClampTop(range.min, s1.size);
	range.max = ClampTop(range.max, s1.size);

	s1.str += range.min;
	s1.size = dim_r1u64(range);
	return s1;
}

B32 str8_cmp(Str8 s1, Str8 s2) {
	Assert(s1.size > 0);
	Assert(s2.size > 0);
	Assert(s1.str);
	Assert(s2.str);

	if (s1.size != s2.size) return false;
	return MemoryMatch(s1.str, s2.str, s1.size);
}

B32 str8_match(Str8 s1, Str8 s2, Str8_MatchFlags flags) {
	B32 result = 1;

	if (s1.size == s2.size && (flags == 0)) {
		return MemoryMatch(s1.str, s2.str, s1.size);
	}

	else if (s1.size == s2.size || (flags & Str8_MatchRightSideSloppy)) {

		B32 case_insensitive = (flags & Str8_MatchCaseInsensitive);
		U64 size = Min(s1.size, s2.size);

		for EachIndex(i, size) {
			U8 s1_char = s1.str[i];
			U8 s2_char = s2.str[i];

			if (case_insensitive) {
				s1_char = upper_from_char(s1_char);
				s2_char = upper_from_char(s2_char);
			}

			if (s1_char != s2_char) {
				result = 0;
				break;
			}
		}
	}
	return result;
}

///////////////////////////////////////
/// cjk: String List Functions 

Str8List str8_list() {
	return (Str8List){0};
}

Str8Node *str8_list_push_node(Str8List *list, Str8Node *node) {
	Assert(list != NULL);
	Assert(node != NULL);

	if (list->last == NULL && list->first == NULL) {
		list->last = node;
		list->first = node;
	} else {
		list->last->next = node;
		list->last = node;
	}

	list->count += 1;
	list->total_size += node->string.size;
	return node;
}

Str8Node *str8_list_push_node_front(Str8List *list, Str8Node *node) {
	Assert(list != NULL);
	Assert(node != NULL);

	if (list->last == NULL && list->first == NULL) {
		list->last = node;
		list->first = node;
	} else {
		node->next = list->first;
		list->first = node;
	}

	list->count += 1;
	list->total_size += node->string.size;

	return node;
}

Str8Node *str8_list_push(Arena *arena, Str8List *list, Str8 string) {
	Assert(arena != NULL);
	Assert(list != NULL);

	Str8Node *node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node){.next = NULL, .string = string};

	return str8_list_push_node(list, node);
}

Str8Node *str8_list_push_front(Arena *arena, Str8List *list, Str8 string) {
	Assert(arena != NULL);
	Assert(list != NULL);

	Str8Node *node = ArenaPushStruct(arena, Str8Node);
	*node = (Str8Node){.next = NULL, .string = string};

	return str8_list_push_node_front(list, node);
}

Str8List *str8_tokenize_list(Arena *arena, Str8 string, Str8 delimiters) {
	Assert(arena != NULL);
	Assert(delimiters.size > 0);

	U64 start_idx = 0;
	Str8List *list = ArenaPushStruct(arena, Str8List);

	for EachIndex(i, string.size) {
		B32 is_delimiter = 0;

		for EachIndex(delim, delimiters.size) {
			if (str8_get(string, i) == str8_get(delimiters, delim)) {
				is_delimiter = 1;
				break;
			}
		}

		if (is_delimiter) {
			Str8 token = str8_substr(string, Rng1_U64(start_idx, i));
			start_idx = i + 1;
			str8_list_push(arena, list, token);
		}
	}

	if (start_idx <= string.size) { // Push final token
		Str8 token = str8_substr(string, Rng1_U64(start_idx, string.size));
		str8_list_push(arena, list, token);
	}

	return list;
}

///////////////////////////////////////
/// cjk: String Array Functions 

Str8Array str8_array_reserve(Arena *arena, U64 capacity){
	NotImplemented;
}
Str8Array str8_tokenize_array(Arena *arena, Str8 string, Str8 delimiters){
	NotImplemented;
}

///////////////////////////////////////
/// cjk: HashMap Functions 

HashMap *hash_map_init(Arena *arena) {
	HashMap *hashmap = ArenaPushStructZero(arena, HashMap);
	hashmap->arena = arena;
	hashmap->next_free_collision_bucket = U64Lit(0);
	return hashmap;
}

void *hash_map_get(HashMap *hash_map, Str8 key) {
	Assert(hash_map);
	Assert(key.size > 0);
	Assert(key.str);

	U64 hash = hash_fnv1a_u64(key.str, key.size);
	hash = hash % DEFAULT_HASH_MAP_SIZE;

	HashBucket bucket = hash_map->data[hash];

	if (str8_cmp(key, bucket.key)) return bucket.data;

	while (bucket.has_next_data) {
		if (str8_cmp(key, bucket.key)) return bucket.data;
		bucket = hash_map->data[bucket.next_data];
	}
	return NULL;
}

void hash_map_put(HashMap *hash_map, Str8 key, void *data) {
	Assert(hash_map);
	Assert(data);
	Assert(key.size > 0);
	Assert(key.str);

	local_persist U64 collisions = 0;
	U64 hash = hash_fnv1a_u64(key.str, key.size);
	hash = hash % DEFAULT_HASH_MAP_SIZE;

	HashBucket *bucket = &hash_map->data[hash];

	// if bucket is empty
	if (bucket->data == NULL) {
		bucket->data = data;
		bucket->key = str8_copy(hash_map->arena, key);
		bucket->has_next_data = false;
		bucket->next_data = 0;
		return;
	}

	printf("Collisions: %lu\n", collisions);
	//
	// if this assert goes off consider increasing the size of the hashmap
	// or changing hash function / choosing a different seed
	Assert(collisions < 1000);
	collisions += 1;

	// traverse collision array until empty bucket is found
	HashBucket *prev = NULL;
	while (bucket->data != NULL) {
		prev = bucket;
		if (bucket->has_next_data) {
			bucket = &hash_map->collisions[bucket->next_data];
			continue;
		}
		break;
	}

	// set previous buckets next data to the current bucket
	prev->has_next_data = true;
	prev->next_data = hash_map->next_free_collision_bucket;

	// insert data
	bucket = &hash_map->collisions[hash_map->next_free_collision_bucket];
	bucket->data = data;
	bucket->key = str8_copy(hash_map->arena, key);
	bucket->has_next_data = false;
	bucket->next_data = 0;

	hash_map->next_free_collision_bucket += 1;
	Assert(hash_map->next_free_collision_bucket <= DEFAULT_HASH_COLLISION_SIZE);
}

// Hashing functions
U64 hash_fnv1a_u64(const void *in_ptr, U64 num_bytes) {
	Assert(num_bytes > 0);
	Assert(in_ptr);

	const U8 *byte_arr = (const U8 *)in_ptr;

	// seeds for FNV-1a hashing do not touch
	U64 fnv_prime = U64Lit(0x00000100000001b3);
	U64 fnv_hash = U64Lit(0xcbf29ce484222325);

	for EachIndex(i, num_bytes) {
		fnv_hash ^= byte_arr[i];
		fnv_hash *= fnv_prime;
	}

	return fnv_hash;
}

///////////////////////////////////////
/// cjk: Profiling Functions

void profile_begin() { 
	global_profiling_begin = clock(); 
}

void profile_end(Str8 message) {
	global_profiling_end = clock();
	F64 time_spent = (F64)(global_profiling_end - global_profiling_begin) / CLOCKS_PER_SEC;
	str8_printf(stderr, "[PROFILING] %.*s: %.4f(s)\n", Str8VArg(message), time_spent);
}

///////////////////////////////////////
/// cjk: Time Functions

DenseTime densetime_from_datetime(DateTime time) {
	DenseTime result = 0;
	result += time.year;
	result *= 12;
	result += time.month;
	result *= 31;
	result += time.day;
	result *= 24;
	result += time.hour;
	result *= 60;
	result += time.min;
	result *= 61;
	result += time.sec;
	result *= 1000;
	result += time.mil_sec;
	return result;
}

DateTime datetime_from_densetime(DenseTime time) {
	DateTime result = {0};
	result.mil_sec = time % 1000;
	time /= 1000;
	result.sec = time % 61;
	time /= 61;
	result.min = time % 60;
	time /= 60;
	result.hour = time % 24;
	time /= 24;
	result.day = time % 31;
	time /= 31;
	result.month_num = time % 12;
	time /= 12;
	Assert(time < max_U32);
	result.year = (U32)time;
	return result;
}

///////////////////////////////////////
/// cjk: CSV Functions

CSV csv_init(Arena *arena, CSV_Config config, char *file_path) {
	Assert(arena);
	Assert(file_path);
	CSV csv_parser = {.file_ptr = fopen(file_path, "rb"),
		    .config = config,
		    .arena = arena,
		    .column_hash_map = {0},
		    .current_row = {0},
		    .rows = 0,
		    .columns = 0};

	Assert(csv_parser.file_ptr);

	return csv_parser;
}

CSV_Row *csv_next_row(CSV *csv) {
	Assert(csv != NULL);

	char *err =
	fgets((char *)csv->row_buffer, sizeof(csv->row_buffer), csv->file_ptr);
	if (err == NULL) return NULL;

	Str8 raw_row = cstring_to_str8((char *)csv->row_buffer);
	csv_row_parse(csv, raw_row);

	return &csv->current_row;
}

void csv_row_parse(CSV *csv, Str8 raw_row) {
	Assert(csv != NULL);

	U64 start_idx = 0;
	Str8 delimiters = csv->config.delimiters;
	Str8 quotes = Str8Lit("\"\'");

	TempArenaScope(temp, csv->arena) {
		csv->current_row.list = ArenaPushStructZero(temp.arena, Str8List);
		*csv->current_row.list = str8_list();

		B32 is_quoted = 0;

		for EachIndex(i, raw_row.size) {
			U8 current_char = str8_get(raw_row, i);

			B32 is_quote_char = 0;
			for EachIndex(quote, quotes.size) {
				if (current_char == str8_get(quotes, quote)) {
					is_quote_char = 1;
					break;
				}
			}

			if (is_quote_char) {
				is_quoted = !is_quoted;
				continue;
			}

			if (is_quoted)break;

			B32 is_delimiter = 0;
			for EachIndex(delim, delimiters.size) {
				if (current_char == str8_get(delimiters, delim)) {
					is_delimiter = 1;
					break;
				}
			}

			if (is_delimiter) {
				Str8 token = str8_substr(raw_row, Rng1_U64(start_idx, i));
				str8_list_push(temp.arena, csv->current_row.list, token);
				start_idx = i + 1;
			}
		}

		if (start_idx <= raw_row.size) { // Push final token
			Str8 token = str8_substr(raw_row, Rng1_U64(start_idx, raw_row.size));
			str8_list_push(temp.arena, csv->current_row.list, token);
		}
	}
}

# ifdef BASE_ENABLE_OS
///////////////////////////////////////
/// cjk: OS API Functions 

#  if OS_LINUX

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

#   elif OS_WINDOWS
#    error "Windows is currently unsupporrted"
#   elif OS_MAC
#    error "Mac is currently unsupported"
#   else
#    error "Unknown OS error"
#   endif
#  endif // BASE_ENABLE_OS


///////////////////////////////////////
/// cjk: Window API Functions 

# ifdef BASE_ENABLE_WINDOW

WM_Context wm_open_window(Arena* arena, RectU16 win_rect, Str8 window_name, U16 border_width, ColorRGBA border_color,  ColorRGBA background_color){
	Assert(arena);	
	WM_Context result = {0};

	result.arena = arena;
	result.size = win_rect;
	result.name = window_name;
	U32 value_mask = 0;
	U32 value_list[2];


	// get connection
	result.connection = xcb_connect(NULL, NULL);
	if(xcb_connection_has_error(result.connection)){
		InvalidPath;
	}

	// get screen
	result.setup = xcb_get_setup(result.connection);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(result.setup);
	result.screen = iter.data;

	// create graphics context
	result.graphics_ctx = xcb_generate_id(result.connection);
	result.window = result.screen->root;
	
	value_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	value_list[0] = result.screen->black_pixel;
	value_list[1] = 0;
	xcb_create_gc(result.connection, result.graphics_ctx, result.window, value_mask, value_list);

	// create window
	result.window = xcb_generate_id(result.connection);
	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	value_list[0] = result.screen->white_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_STRUCTURE_NOTIFY

	xcb_create_window(result.connection, 
		   XCB_COPY_FROM_PARENT,
		   result.window,
		   result.screen->root,
		   win_rect.x,
		   win_rect.y,
		   win_rect.width,
		   win_rect.height,
		   border_width,
		   XCB_WINDOW_CLASS_INPUT_OUTPUT,
		   result.screen->root_visual,
		   value_mask,
		   value_list);

	// set window name
	xcb_change_property(result.connection,
		     XCB_PROP_MODE_REPLACE,
		     result.window,
		     XCB_ATOM_WM_NAME,
		     XCB_ATOM_STRING, 
		     8,
		     window_name.size,
		     window_name.str);


	Assert(result.connection);
	Assert(result.screen);

	result.image = XShmCreateImage(result.display,
				DefaultVisualOfScreen(result.screen),
				DefaultDepthOfScreen(result.screen),
				ZPixmap,
				NULL,
				&result.shm_info,
				result.over_size.width, result.over_size.height);

	Assert(result.image);

	// create shared memory region
	U64 total_size = result.image->bytes_per_line * result.image->height;
	result.shared_mem_seg = xcb_generate_id(result.connection);
	result.shared_mem_id = shmget(IPC_PRIVATE, total_size, IPC_CREAT|0600);

	result.shm_info.shmaddr = result.image->data = shmat(result.shm_info.shmid, 0, 0);
	result.shm_info.readOnly = False;

	Status status = XShmAttach(result.display, &result.shm_info);
	XSync(result.display, False);
	shmctl(result.shm_info.shmid, IPC_RMID, 0);

	Assert(status);

	xcb_map_window(result.connection, result.window);
	return result;	
}

void wm_resize_window(WM_Context* ctx, U16 width, U16 height){
	XResizeWindow(ctx->display, ctx->window, width, height);
}

void wm_move_window(WM_Context* ctx, U16 x, U16 y){
	XMoveWindow(ctx->display, ctx->window, x, y);
}


void wm_resize_and_move_window(WM_Context* ctx, RectU16 new_size){
	U32 change_mask = WM_WindowCfgUpdate_X | WM_WindowCfgUpdate_Y | WM_WindowCfgUpdate_Width | WM_WindowCfgUpdate_Height;
	XWindowChanges changes = {
		.x = new_size.x,
		.y = new_size.y,
		.width = new_size.width,
		.height = new_size.height
	};
	XConfigureWindow(ctx->display, ctx->window, change_mask, &changes);
}


void wm_draw_window(WM_Context* ctx){
	Assert(ctx);
	Assert(ctx->display);
	Assert(ctx->window);
	Assert(ctx->image);

#if HAS_SYS_SHM	
	XShmPutImage(ctx->display, ctx->window, ctx->graphics_ctx, ctx->image, 0, 0, 0, 0, ctx->size.width, ctx->size.height, False);
	XSync(ctx->display, False);
#else
	XPutImage(ctx->display, ctx->window, ctx->graphics_ctx, ctx->image, 0, 0, 0, 0, ctx->size.width, ctx->size.height);
	XFlush(ctx->display);
#endif
}

// 2d primitive drawing
void wm_draw_rect(WM_Context* ctx, RectF32 rect, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	
	U32 x1 = Max(0, rect.x);
	U32 y1 = Max(0, rect.y);
	U32 x2 = Min(ctx->size.width, (rect.x + rect.width));
	U32 y2 = Min(ctx->size.height, (rect.y + rect.height));

	if( x1 >= x2 || y1 >= y2 ) return;

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	U32 width = x2 - x1;
	U32 color_const = color_rgba_to_bgra(color).c;

	// this points to the first pixel in the rect
	U32* row_ptr = pixels + (y1 * stride) + x1;

	for (U32 y = y1; y < y2; y++){
		for (U32 x = 0; x < width; x++){
			row_ptr[x] = color_const; 
		}
		row_ptr += stride;
	}
}


// This method uses Midpoint circle algorithm for quickly drawing a circle
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm 
void wm_draw_circle(WM_Context* ctx, Vec2F32 center, F32 radius, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);
	Assert(radius > 0.0);

	S32 x0 = (S32) floorf(center.x);
	S32 y0 = (S32) floorf(center.y);
	S32 r  = (S32) radius;

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	S32 window_width = (S32)ctx->size.width;
	S32 window_height = (S32)ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	S32 x = r;
	S32 y = 0;
	S32 decision = 1 - r;

	#define LOCAL_MACRO_DRAWSPAN(py, x_start, x_end) do{\
			if((py) >= (0) && (py) <= (window_height)) {\
      				S32 left = Max(0, (x_start));\
				S32 right = Min((window_width-1),(x_end));\
				for(S32 px = left; px < right; px ++){\
					pixels[(py) * stride + px] = color_const;\
				}\
			}\
		}while(0)

	while(x >= y){
		LOCAL_MACRO_DRAWSPAN(y0+y, x0-x, x0+x);
		LOCAL_MACRO_DRAWSPAN(y0-y, x0-x, x0+x);
		LOCAL_MACRO_DRAWSPAN(y0+x, x0-y, x0+y);
		LOCAL_MACRO_DRAWSPAN(y0-x, x0-y, x0+y);

		y++;
		if(decision <= 0){
			decision += 2 * y + 1;
		}else{
			x--;
			decision += 2 * (y - x) + 1;
		}
	}
	#undef Local_Macro_DRAWSPAN
} 

// This method uses Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm  
// https://groups.csail.mit.edu/graphics/classes/6.837/F02/lectures/6.837-7_Line.pdf
// https://zingl.github.io/Bresenham.pdf
void wm_draw_line(WM_Context* ctx, Vec2F32 p1, Vec2F32 p2, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	S32 window_width = ctx->size.width;
	S32 window_height = ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	S32 x0 = (S32) floorf(p1.x);
	S32 y0 = (S32) floorf(p1.y); 
	S32 x1 = (S32) floorf(p2.x); 
	S32 y1 = (S32) floorf(p2.y);


	S32 dx = abs(x1 - x0);
	S32 dy = -abs(y1 - y0);
	S32 sx = (x0 < x1)? 1 : -1;
	S32 sy = (y0 < y1)? 1 : -1;
	S32 err = dx+dy;
	S32 err_2;

	for(;;){
		if( 0 <= x0 && x0 < window_width && 0 <= y0 && y0 < window_height){
			pixels[y0 * stride + x0] = color_const;
		}
		
		err_2 = 2 * err;

		if(err_2 >= dy){
			if(x0 == x1) break;
			err += dy;
			x0 += sx;
		}
		if(err_2 <= dx){
			if(y0 == y1) break;
			err += dx;
			y0 += sy;
		}
	}
}

void wm_draw_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	wm_draw_line(ctx, v1, v2, color);
	wm_draw_line(ctx, v2, v3, color);
	wm_draw_line(ctx, v3, v1, color);
} 

B32 wm_is_point_in_triangle(Vec2F32 point, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3){
	B32 result = false;	
		 		
	return result;	
}

void wm_draw_filled_triangle(WM_Context* ctx, Vec2F32 v1, Vec2F32 v2, Vec2F32 v3, ColorRGBA color){
	Assert(ctx);
	Assert(ctx->image);

	U32* pixels = (U32*) ctx->image->data;
	U32 stride = ctx->image->bytes_per_line / sizeof(ColorRGBA);
	U32 window_width = ctx->size.width;
	U32 window_height = ctx->size.height;
	U32 color_const = color_rgba_to_bgra(color).c;

	// get bounding box
	U32 x_min = Min(0, Min(v1.x, Min(v2.x, v3.x)));
	U32 y_min = Min(0, Min(v1.y, Min(v2.y, v3.y)));
	U32 x_max = Min(window_width, Max(v1.x, Max(v2.x, v3.x)));
	U32 y_max = Min(window_height, Max(v1.y, Max(v2.y, v3.y)));

	Rng1U32 x_rng = Rng1_U32(x_min, x_max);
	Rng1U32 y_rng = Rng1_U32(y_min, y_max);

	for EachInRange(y, y_rng){
		for EachInRange(x, x_rng){
			if(wm_is_point_in_triangle(Vec2_F32(x, y), v1, v2, v3)){
				pixels[y * stride + x] = color_const;
			}
		}	
	}
}


void wm_close_window(WM_Context* ctx){
	XUnmapWindow(ctx->display, ctx->window);
	XDestroyWindow(ctx->display, ctx->window);
	XFreeGC(ctx->display, ctx->graphics_ctx);
	XCloseDisplay(ctx->display);
}

void wm_register_input_events(WM_Context* ctx, WM_EventFlag flags){
	long event_mask = 0;	

	for EachIndex(i, ArrayCount(wm_event_mask_map)){
		if(flags & wm_event_mask_map[i].flag){
			event_mask |= wm_event_mask_map[i].event_mask;
		}
	}
	
	XSelectInput(ctx->display, ctx->window, event_mask);
}

U32 wm_num_of_pending_events(WM_Context* ctx){
	return XPending(ctx->display);
}


# endif // BASE_ENABLE_WINDOW
#endif // BASE_IMPLEMENTATION

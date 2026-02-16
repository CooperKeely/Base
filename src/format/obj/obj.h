#ifndef FORMAT_OBJ_H
#define FORMAT_OBJ_H

///////////////////////////////////////
/// cjk: OBJ File Parser 

typedef U64 FMT_OBJ_LineType;
enum{
	FMT_OBJ_LineType_Vertex, 
	FMT_OBJ_LineType_Texture,
	FMT_OBJ_LineType_Normal,
	FMT_OBJ_LineType_Face,
	FMT_OBJ_LineType_Group,
	FMT_OBJ_LineType_Material,
	FMT_OBJ_LineType_MaterialLibrary,
	FMT_OBJ_LineType_Comment,
	FMT_OBJ_LineType_Malformed,
	FMT_OBJ_LineType_Empty,
	FMT_OBJ_LineType_Count,
};

StaticAssert(FMT_OBJ_LineType_Count == 10, line_type_broken);

typedef struct{
	U32 v_idx;
	U32 vt_idx;
	U32 vn_idx;
} FMT_OBJ_FaceCorner;

typedef struct{
	FMT_OBJ_LineType line_type;
	union{
		Vec4F32 v;
		Vec3F32 vt;
		Vec3F32 vn;

		struct {
			FMT_OBJ_FaceCorner corners[3];
		} face;	

		Str8 string_data;
	};	
} FMT_OBJ_Line;

typedef struct{
	FMT_OBJ_Line* array;
	Arena* arena;
	U64 count;
	U64 capacity;
} FMT_OBJ_LineList;

typedef struct {
	OS_Handle file_handle;
	Arena *arena;
	
	// TODO: implement .mtl file parsing and group support
	// FMT_MTL mat_lib;
	// FMT_MTL_Material current_material;
	// FMT_OBJ_Group current_group;

	FMT_OBJ_LineList vertex_list;
	FMT_OBJ_LineList texture_list;
	FMT_OBJ_LineList normal_list;
	FMT_OBJ_LineList face_list;
	FMT_OBJ_LineList malformed_list;
} FMT_OBJ_Object;

FMT_OBJ_Object* fmt_obj_object_init(Arena *arena, Str8 file_path);

// line parsing functions
FMT_OBJ_Line fmt_obj_parse_line(Str8 line);
FMT_OBJ_Line fmt_obj_parse_vertex(Str8 line);
FMT_OBJ_Line fmt_obj_parse_texture(Str8 line);
FMT_OBJ_Line fmt_obj_parse_normal(Str8 line);
FMT_OBJ_Line fmt_obj_parse_face(Str8 line);
FMT_OBJ_Line fmt_obj_parse_group(Str8 line);
FMT_OBJ_Line fmt_obj_parse_material(Str8 line);
FMT_OBJ_Line fmt_obj_parse_material_library(Str8 line);
FMT_OBJ_Line fmt_obj_parse_comment(Str8 line);
FMT_OBJ_Line fmt_obj_parse_malformed(Str8 line);
FMT_OBJ_Line fmt_obj_parse_empty(Str8 line);

// line list functions
FMT_OBJ_LineList fmt_obj_line_list_init(Arena* arena, U64 size);
void fmt_obj_line_list_append(FMT_OBJ_LineList* list, FMT_OBJ_Line line);
void fmt_obj_line_list_resize(FMT_OBJ_LineList* list);
FMT_OBJ_Line* fmt_obj_line_list_get(FMT_OBJ_LineList* list, U64 index);

#endif // FORMAT_OBJ_H

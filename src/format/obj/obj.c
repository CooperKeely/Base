///////////////////////////////////////
/// cjk: OBJ File Parser 

FMT_OBJ_Object* fmt_obj_object_init(Arena *arena, Str8 file_path){
	FMT_OBJ_Object* ret = ArenaPushStruct(arena, FMT_OBJ_Object);
	ret->file_handle = os_file_open(file_path, OS_AccessFlag_Read); 
	ret->arena = arena;	
	return ret;
}

// line parsing functions
FMT_OBJ_Line fmt_obj_parse_line(Str8* line){
	Assert(line);
	FMT_OBJ_Line ret;
	
	S64 first_space = str8_find_first_char(*line, ' ');
	if(first_space < 0) ret = fmt_obj_parse_malformed(line);

	Str8 prefix = str8_get_slice(*line, 0, first_space);

	if(str8_cmp(prefix, Str8Lit("v"))) 		ret = fmt_obj_parse_vertex(line);
	else if(str8_cmp(prefix, Str8Lit("vt"))) 	ret = fmt_obj_parse_texture(line);
	else if(str8_cmp(prefix, Str8Lit("vn"))) 	ret = fmt_obj_parse_normal(line);
	else if(str8_cmp(prefix, Str8Lit("vf"))) 	ret = fmt_obj_parse_face(line);
	else if(str8_cmp(prefix, Str8Lit("g"))) 	ret = fmt_obj_parse_group(line);
	else if(str8_cmp(prefix, Str8Lit("usemtl"))) 	ret = fmt_obj_parse_material(line);
	else if(str8_cmp(prefix, Str8Lit("mtllib"))) 	ret = fmt_obj_parse_material_library(line);
	else if(str8_cmp(prefix, Str8Lit("#"))) 	ret = fmt_obj_parse_comment(line);	
	else return 					ret = fmt_obj_parse_malformed(line); 
}

FMT_OBJ_Line fmt_obj_parse_vertex(Str8* line){



}

FMT_OBJ_Line fmt_obj_parse_texture(Str8* line){



}

FMT_OBJ_Line fmt_obj_parse_normal(Str8* line){



}

FMT_OBJ_Line fmt_obj_parse_face(Str8* line){



}

FMT_OBJ_Line fmt_obj_parse_group(Str8* line){NotImplemented;}
FMT_OBJ_Line fmt_obj_parse_material(Str8* line){NotImplemented;}

FMT_OBJ_Line fmt_obj_parse_material_library(Str8* line){NotImplemented;}

FMT_OBJ_Line fmt_obj_parse_comment(Str8* line){
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Comment;
	ret.string_data = *line;
	return ret;
}

FMT_OBJ_Line fmt_obj_parse_malformed(Str8* line){
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Malformed;
	ret.string_data = *line;
	return ret;
}

// line list functions
FMT_OBJ_LineList fmt_objt_line_list_init(Arena* arena){



}

void fmt_obj_line_list_append(FMT_OBJ_LineList* list, FMT_OBJ_Line* line){



}

void fmt_obj_line_list_resize(FMT_OBJ_LineList* list){



}

FMT_OBJ_Line* fmt_obj_line_list_get(FMT_OBJ_LineList* list, U64 index){



}



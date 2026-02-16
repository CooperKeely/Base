///////////////////////////////////////
/// cjk: OBJ File Parser 

FMT_OBJ_Object* fmt_obj_object_init(Arena *arena, Str8 file_path){
	FMT_OBJ_Object* ret = ArenaPushStructZero(arena, FMT_OBJ_Object);
	ret->arena = arena;	

	// open a file map
	ret->file_handle = os_file_open(file_path, OS_AccessFlag_Read); 
	OS_FileProperties file_props = os_properties_from_file_handle(ret->file_handle);
	U8* file_buf = os_file_map_view_open(ret->file_handle, OS_AccessFlag_Read, Rng1_U64(0, file_props.size));

	U64 num_verticies = 0;
	U64 num_normals = 0;
	U64 num_textures = 0;
	U64 num_faces = 0;

	for(U64 i = 0; i < file_props.size;){	
		// find the end of the line
		U64 end = i;
		while (end < file_props.size && file_buf[end] != '\n' && file_buf[end] != '\r') end ++;
		
		// get a slice of the line
		Str8 line_slice = str8(&file_buf[i], end-i);
		line_slice = str8_trim_whitespace(line_slice);	
		if(line_slice.size == 0 || line_slice.str[0] == '#') goto next_line;		

		// parse prefix 
		S64 first_space = str8_find_first_char(line_slice, ' ');
		Str8 prefix;
		if(first_space < 0){
			prefix = line_slice;
		} else{
			prefix = str8_get_slice(line_slice, 0, first_space);
		}

		if(str8_cmp(prefix, Str8Lit("v")))		{ num_verticies++; }
		else if(str8_cmp(prefix, Str8Lit("vt")))	{ num_textures++; }	
		else if(str8_cmp(prefix, Str8Lit("vn")))	{ num_normals++; } 	
		else if(str8_cmp(prefix, Str8Lit("f")))		{ num_faces++; }
		//else if(str8_cmp(prefix, Str8Lit("g"))) 	
		//else if(str8_cmp(prefix, Str8Lit("usemtl"))) 	
		//else if(str8_cmp(prefix, Str8Lit("mtllib"))) 	

		next_line:
		// iterate through the new line characters
		i = end;
		while (i < file_props.size && (file_buf[i] == '\n' || file_buf[i] == '\r')) i ++;
	}
	
	// unmap the file
	os_file_map_view_close(ret->file_handle, file_buf, Rng1_U64(0, file_props.size));	

	// allocate exactly what each needs
	fmt_obj_line_array_init(&ret->vertex_array, ret->arena, num_verticies);
	fmt_obj_line_array_init(&ret->normal_array, ret->arena, num_normals);
	fmt_obj_line_array_init(&ret->texture_array, ret->arena, num_textures);
	fmt_obj_line_array_init(&ret->face_array, ret->arena, num_faces);
	
	// TODO: (cjk): add a automatic array resizing for the malformed array
	fmt_obj_line_array_init(&ret->malformed_array, ret->arena, KB(1));

	return ret;
}

void fmt_obj_parse_file(FMT_OBJ_Object* obj_file){

	OS_Handle fd = obj_file->file_handle;
	OS_FileProperties file_props = os_properties_from_file_handle(fd);
	U8* file_buf = os_file_map_view_open(fd, OS_AccessFlag_Read, Rng1_U64(0, file_props.size));

	for(U64 i = 0; i < file_props.size;){	
		U64 end = i;
		while (end < file_props.size && file_buf[end] != '\n' && file_buf[end] != '\r') end ++;

		Str8 line_slice = str8(&file_buf[i], end - i);	
		FMT_OBJ_Line line = fmt_obj_parse_line(line_slice);	
		
		switch(line.line_type){
			case FMT_OBJ_LineType_Vertex: {
				fmt_obj_line_array_append(&obj_file->vertex_array, line);
			} break;
			case FMT_OBJ_LineType_Texture: {
				fmt_obj_line_array_append(&obj_file->texture_array, line);
			} break;
			case FMT_OBJ_LineType_Normal: { 
				fmt_obj_line_array_append(&obj_file->normal_array, line);
			} break;
			case FMT_OBJ_LineType_Face: {
				fmt_obj_line_array_append(&obj_file->face_array, line);
			} break;
			case FMT_OBJ_LineType_Malformed: {
				fmt_obj_line_array_append(&obj_file->malformed_array, line);
			} break;
			case FMT_OBJ_LineType_Group: 
			case FMT_OBJ_LineType_Material: 
			case FMT_OBJ_LineType_MaterialLibrary: 
			case FMT_OBJ_LineType_Comment: 
			case FMT_OBJ_LineType_Empty: 
			case FMT_OBJ_LineType_Count: 
			default:{
			}break;
		}

		i = end;
		while (i < file_props.size && (file_buf[i] == '\n' || file_buf[i] == '\r')) i ++;
	}

	os_file_map_view_close(fd, file_buf, Rng1_U64(0, file_props.size));
}


// line parsing functions
FMT_OBJ_Line fmt_obj_parse_line(Str8 line){
	FMT_OBJ_Line ret;
	if(line.size == 0) return fmt_obj_parse_empty(line);

	// chopp off trailing comment
	S64 first_comment = str8_find_first_char(line, '#');
	if(first_comment == 0) return fmt_obj_parse_comment(line);
	else if (first_comment > 0){
		line = str8_get_slice(line, 0, first_comment);	
	}

	// trim whitespace
	line = str8_trim_whitespace(line);
	if(line.size == 0) return fmt_obj_parse_empty(line);
	
	// parse line
	S64 first_space = str8_find_first_char(line, ' ');
	Str8 prefix;
	if(first_space < 0){
		prefix = line;	
	}else{
		prefix = str8_get_slice(line, 0, first_space);
	} 
	
	prefix = str8_trim_whitespace(prefix);
	if(str8_cmp(prefix, Str8Lit("v"))) {
		ret = fmt_obj_parse_vertex(line);
	}
	else if(str8_cmp(prefix, Str8Lit("vt"))) 	{
		ret = fmt_obj_parse_texture(line);
	}
	else if(str8_cmp(prefix, Str8Lit("vn"))) 	{
		ret = fmt_obj_parse_normal(line);
	}
	else if(str8_cmp(prefix, Str8Lit("f"))) 	{
		ret = fmt_obj_parse_face(line);
	}
	else if(str8_cmp(prefix, Str8Lit("g"))) 	{
		ret = fmt_obj_parse_group(line);
	}
	else if(str8_cmp(prefix, Str8Lit("usemtl"))) 	{
		ret = fmt_obj_parse_material(line);
	}
	else if(str8_cmp(prefix, Str8Lit("mtllib"))) 	{
		ret = fmt_obj_parse_material_library(line);
	}
	else {
		ret = fmt_obj_parse_malformed(line);
	}

	return ret;
}

FMT_OBJ_Line fmt_obj_parse_vertex(Str8 line){
	Assert(line.str);
	Assert(line.size > 0);

	FMT_OBJ_Line ret = {0};

	ret.line_type = FMT_OBJ_LineType_Vertex;
	
	ScratchArenaScope(scratch, 0, 0){
		Str8List* token_list = str8_tokenize_list(scratch.arena, line, Str8Lit(" "));
		

		Str8 x = str8_list_get(token_list, 1);
		Str8 y = str8_list_get(token_list, 2);
		Str8 z = str8_list_get(token_list, 3);
		Str8 w = str8_list_get(token_list, 4);

		// if not found return a malformed line
		if(x.size == 0 || y.size == 0 || z.size == 0){
			return fmt_obj_parse_malformed(line);
		}
		
		// mandatory 	
		ret.v.x = str8_to_f32(x);
		ret.v.y = str8_to_f32(y);
		ret.v.z = str8_to_f32(z);
		
		// optional
		ret.v.w = (w.size > 0)? str8_to_f32(w) : F32Lit(1.0);
	}

	return ret;
}

FMT_OBJ_Line fmt_obj_parse_texture(Str8 line){
	FMT_OBJ_Line ret = {0};

	ret.line_type = FMT_OBJ_LineType_Texture;
	
	ScratchArenaScope(scratch, 0, 0){
		Str8List* token_list = str8_tokenize_list(scratch.arena, line, Str8Lit(" "));

		Str8 x = str8_list_get(token_list, 1);
		Str8 y = str8_list_get(token_list, 2);
		Str8 z = str8_list_get(token_list, 3);
		
		// mandotory
		if(x.size == 0) return fmt_obj_parse_malformed(line);
		else ret.vt.x = str8_to_f32(x);

		// optional
		ret.vt.y = (y.size > 0)? str8_to_f32(y) : F32Lit(0.0);
		ret.vt.z = (z.size > 0)? str8_to_f32(z) : F32Lit(0.0);
	}

	return ret;
}

FMT_OBJ_Line fmt_obj_parse_normal(Str8 line){
	Assert(line.str);
	Assert(line.size > 0);
	FMT_OBJ_Line ret = {0};

	ret.line_type = FMT_OBJ_LineType_Normal;
	
	ScratchArenaScope(scratch, 0, 0){
		Str8List* token_list = str8_tokenize_list(scratch.arena, line, Str8Lit(" "));

		Str8 x = str8_list_get(token_list, 1);
		Str8 y = str8_list_get(token_list, 2);
		Str8 z = str8_list_get(token_list, 3);
		
		// if not found return a malformed line
		if(x.size == 0 || y.size == 0 || z.size == 0){
			return fmt_obj_parse_malformed(line);
		}
		
		ret.vn.x = str8_to_f32(x);
		ret.vn.y = str8_to_f32(y);
		ret.vn.z = str8_to_f32(z);
	}

	return ret;
}

FMT_OBJ_Line fmt_obj_parse_face(Str8 line){
	// TODO: (cjk): add the face parsing logic
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Face;
	return ret;
}

FMT_OBJ_Line fmt_obj_parse_group(Str8 line){
	// TODO: (cjk): add the group parsing logic
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Group;
	return ret;

}

FMT_OBJ_Line fmt_obj_parse_material(Str8 line){
	// TODO: (cjk): add the material parsing logic
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Material;
	return ret;
}

FMT_OBJ_Line fmt_obj_parse_material_library(Str8 line){
	// TODO: (cjk): add the material library parsing logic
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_MaterialLibrary;
	return ret;
}

FMT_OBJ_Line fmt_obj_parse_comment(Str8 line){
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Comment;
	ret.string_data = line;
	return ret;
}

FMT_OBJ_Line fmt_obj_parse_malformed(Str8 line){
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Malformed;
	ret.string_data = line;
	return ret;
}

FMT_OBJ_Line fmt_obj_parse_empty(Str8 line){
	FMT_OBJ_Line ret;
	ret.line_type = FMT_OBJ_LineType_Empty;
	ret.string_data = (Str8){0};
	return ret;
}

// line list functions
void fmt_obj_line_array_init(FMT_OBJ_LineArray* list, Arena* arena, U64 size){
	list->array = ArenaPushArrayZero(arena, FMT_OBJ_Line, size);
	list->capacity = size;
	list->count = 0;
}

void fmt_obj_line_array_append(FMT_OBJ_LineArray* list, FMT_OBJ_Line line){
	Assert(list->count <= list->capacity);
	list->array[list->count] = line;
	list->count ++;
}

FMT_OBJ_Line* fmt_obj_line_array_get(FMT_OBJ_LineArray* list, U64 index){
	Assert(index < list->count);
	return &list->array[index];
}

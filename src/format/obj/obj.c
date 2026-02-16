///////////////////////////////////////
/// cjk: OBJ File Parser 

FMT_OBJ_Object* fmt_obj_object_init(Arena *arena, Str8 file_path){
	FMT_OBJ_Object* ret = ArenaPushStruct(arena, FMT_OBJ_Object);
	ret->file_handle = os_file_open(file_path, OS_AccessFlag_Read); 
	ret->arena = arena;	


	U64 num_verticies;
	U64 num_normals;
	U64 num_textures;
	U64 num_faces;

	ScratchArenaScope(scratch, 0, 0){

		U64 line_size = KB(1);

		U8* line_buffer = ArenaPushArray(scratch.arena, U8, line_size);

		S64 bytes_read = 0;		
		U64 start_read= 0;
		U64 end_read = buffer_size;

		do{
			bytes_read = os_file_read_data(ret->file_handle, 
				  			Rng1_U64(start_read, end_read), 
				  			(void*) line_buffer);
			Assert(0 <= bytes_read);

			U64 len = 0;
			for (; line_buffer[len] != '\n' || len < bytes_read; len ++);
		
			Str8 line_slice = (str8){
				.size = len,
				.str = line_buffer,
			};

			Str8List* str8_tokenize_list(scratch.arena, line_slice, Str8Lit(' '));


			start_read = end_read;
			end_read += len;
		}while(bytes_read != 0);


			
		

	}		
	

	return ret;
}

// line parsing functions
FMT_OBJ_Line fmt_obj_parse_line(Str8 line){
	FMT_OBJ_Line ret;
	
	S64 first_space = str8_find_first_char(line, ' ');
	if(first_space < 0) ret = fmt_obj_parse_empty(line);

	Str8 prefix = str8_get_slice(line, 0, first_space);

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

FMT_OBJ_Line fmt_obj_parse_vertex(Str8 line){
	FMT_OBJ_Line ret = {0};

	ret.line_type = FMT_OBJ_LineType_Vertex;
	
	ScratchArenaScope(scratch, 0, 0){
		Str8List* token_list = str8_tokenize_list(scratch.arena, line, Str8Lit(' '));
		
		Str8 x = str8_list_get(token_list, 1);
		Str8 y = str8_list_get(token_list, 2);
		Str8 z = str8_list_get(token_list, 3);
		Str8 w = str8_list_get(token_list, 4);
		
		// if not found return a malformed line
		if(x.size == 0 || y.size == 0 || z.size == 0){
			return fmt_obj_parse_malformed(line);
		}
		
		ret.v.x = str8_to_f32(x);
		ret.v.y = str8_to_f32(y);
		ret.v.z = str8_to_f32(z);

		if(w.size == 0) ret.v.w = F32Lit(1.0); // w defaults to 1.0f
		else ret.v.w = str8_to_f32(w);
	}

	return ret;
}

FMT_OBJ_Line fmt_obj_parse_texture(Str8 line){
	FMT_OBJ_Line ret = {0};

	ret.line_type = FMT_OBJ_LineType_Texture;
	
	ScratchArenaScope(scratch, 0, 0){
		Str8List* token_list = str8_tokenize_list(scratch.arena, line, Str8Lit(' '));

		Str8 x = str8_list_get(token_list, 1);
		Str8 y = str8_list_get(token_list, 2);
		Str8 z = str8_list_get(token_list, 3);
		
		// if not found return a malformed line
		if(x.size == 0) return fmt_obj_parse_malformed(line);
		else ret.vt.x = str8_to_f32(x);

		// y defaults to 0.0f
		if(y.size == 0) ret.vt.y = F32Lit(0.0); 
		else ret.vt.y = str8_to_f32(y);

		// z defaults to 0.0f
		if(z.size == 0) ret.vt.z = F32Lit(0.0); 
		else ret.vt.z = str8_to_f32(z);
	}

	return ret;
}

FMT_OBJ_Line fmt_obj_parse_normal(Str8 line){
	FMT_OBJ_Line ret = {0};

	ret.line_type = FMT_OBJ_LineType_Normal;
	
	ScratchArenaScope(scratch, 0, 0){
		Str8List* token_list = str8_tokenize_list(scratch.arena, line, Str8Lit(' '));

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

FMT_OBJ_Line fmt_obj_parse_face(Str8 line){NotImplemented;}
FMT_OBJ_Line fmt_obj_parse_group(Str8 line){NotImplemented;}
FMT_OBJ_Line fmt_obj_parse_material(Str8 line){NotImplemented;}
FMT_OBJ_Line fmt_obj_parse_material_library(Str8 line){NotImplemented;}

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
FMT_OBJ_LineList fmt_obj_line_list_init(Arena* arena, U64 size){
	FMT_OBJ_LineList ret = {0};
	ret.arena = arena;
	
	// initial size 1028
	ret.array = ArenaPushArray(arena, FMT_OBJ_Line, KB(1));
	
	ret.capacity = KB(1);
	ret.count = 0;
	return ret;
}

void fmt_obj_line_list_append(FMT_OBJ_LineList* list, FMT_OBJ_Line line){
	if(list->count >= list->capacity){
		fmt_obj_line_list_resize(list);
	}
	Assert(list->count < list->capacity);

	list->array[list->count] = line;
	list->count ++;
}

FMT_OBJ_Line* fmt_obj_line_list_get(FMT_OBJ_LineList* list, U64 index){
	Assert(index < list->count);
	return &list->array[index];
}



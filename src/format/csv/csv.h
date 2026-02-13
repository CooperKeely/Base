#ifndef FORMAT_CSV_H
#define FORMAT_CSV_H
///////////////////////////////////////
/// cjk: CSV Parser Definitions 

typedef struct {
	Str8 delimiters;
	B32 headers_enabled;
} CSV_Config;

typedef struct {
	Str8List *list;
	U64 index;
} CSV_Row;

typedef struct {
	FILE *file_ptr;
	Arena *arena;
	CSV_Config config;
	HashMap column_hash_map;
	CSV_Row current_row;
	U8 row_buffer[KB(16)];
	U64 rows;
	U64 columns;
} CSV;

#define EachCSVRow(csv_ptr, row_ptr) 	(CSV_Row *row_ptr = csv_next_row(csv_ptr); row_ptr != NULL; row_ptr = csv_next_row(csv_ptr))

CSV csv_init(Arena *arena, CSV_Config config, char *file_path);
CSV_Row *csv_next_row(CSV *csv);
void csv_row_parse(CSV *csv, Str8 raw_row);


#endif FORMAT_CSV_H

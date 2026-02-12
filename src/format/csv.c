
#include "csv.h"

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

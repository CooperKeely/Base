#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {	
	TOK_TokenType_None,

	TOK_TokenType_OpenParen,
	TOK_TokenType_CloseParen,
	TOK_TokenType_Colon,
	TOK_TokenType_Semicolon,
	TOK_TokenType_Asterisk,
	TOK_TokenType_OpenBracket,
	TOK_TokenType_CloseBracket,
	TOK_TokenType_OpenBrace,
	TOK_TokenType_CloseBrace,
	TOK_TokenType_Equals,
	TOK_TokenType_Comma,
	TOK_TokenType_Or,
	TOK_TokenType_Pound,
	TOK_TokenType_Period,

	TOK_TokenType_String,
	TOK_TokenType_Identifier,
	TOK_TokenType_Number,

	TOK_TokenType_Spacing,
	TOK_TokenType_EndOfLine,
	TOK_TokenType_Comment,
	TOK_TokenType_EndOfStream,

	TOK_TokenType_COUNT
} TOK_TokenType;



typedef struct{
	Str8 file_name;
	S32 column_number;
	S32 line_number;

	TOK_TokenType type;
	Str8 text;
	F32 num_real;
	S32 num_integer;
} TOK_Token;

typedef struct {
	Str8 file_name;
	S32 current_column_number;
	S32 current_line_number;

} TOK_Tokenizer;


#endif // FORMAT_TOKENIZER_H

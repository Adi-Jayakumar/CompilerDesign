#pragma once
#include "token.h"
#include <fstream>

class Lexer
{
    std::string identifier_str; // Filled in if IDENT
    std::string string_val;     // Filled in if String Literal
    size_t line_no, column_no;
    std::ifstream file;
    bool ended;

    Token ReturnToken(const std::string &lex_val, const int tok_type) const;

public:
    int int_val;     // Filled in if INT_LIT
    bool bool_val;   // Filled in if BOOL_LIT
    float float_val; // Filled in if FLOAT_LIT
    Lexer(std::ifstream &_file) : line_no(1), column_no(1), file(std::move(_file)), ended(false)
    {
        file.clear();
        file.seekg(0, file.beg);
    };
    ~Lexer() { file.close(); };
    Token GetToken();
};
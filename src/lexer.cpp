#include "lexer.h"
#include <iostream>

Token Lexer::ReturnToken(const std::string &lex_val, const int tok_type) const
{
    return Token(static_cast<TOKEN_TYPE>(tok_type), lex_val, line_no, column_no - lex_val.length() - 1);
}

Token Lexer::GetToken()
{
    static char LastChar = ' ';
    static char NextChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar) && file.peek() != EOF)
    {
        if (LastChar == '\n')
        {
            ++line_no;
            column_no = 1;
        }
        file.get(LastChar);
        column_no++;
    }

    if (ended)
        return ReturnToken("", EOF_TOK);

    if (file.peek() == EOF)
        ended = true;

    if (isalpha(LastChar) || LastChar == '_')
    { // identifier: [a-zA-Z_][a-zA-Z_0-9]*
        identifier_str = "";
        column_no++;

        while (isalnum(LastChar) || LastChar == '_')
        {
            identifier_str += LastChar;
            column_no++;
            file.get(LastChar);
        }

        if (identifier_str == "int")
            return ReturnToken("int", INT_TOK);
        else if (identifier_str == "bool")
            return ReturnToken("bool", BOOL_TOK);
        else if (identifier_str == "float")
            return ReturnToken("float", FLOAT_TOK);
        else if (identifier_str == "void")
            return ReturnToken("void", VOID_TOK);
        else if (identifier_str == "extern")
            return ReturnToken("extern", EXTERN);
        else if (identifier_str == "if")
            return ReturnToken("if", IF);
        else if (identifier_str == "else")
            return ReturnToken("else", ELSE);
        else if (identifier_str == "while")
            return ReturnToken("else", WHILE);
        else if (identifier_str == "return")
            return ReturnToken("return", RETURN);
        else if (identifier_str == "true")
        {
            bool_val = true;
            return ReturnToken("true", BOOL_LIT);
        }
        else if (identifier_str == "false")
        {
            bool_val = false;
            return ReturnToken("false", BOOL_LIT);
        }

        return ReturnToken(identifier_str, IDENT);
    }

    if (LastChar == '=')
    {
        file.get(NextChar);
        if (NextChar == '=')
        { // EQ: ==
            file.get(LastChar);
            column_no += 2;
            return ReturnToken("==", EQ);
        }
        else
        {
            LastChar = NextChar;
            column_no++;
            return ReturnToken("=", ASSIGN);
        }
    }

    if (LastChar == '{')
    {
        file.get(LastChar);
        column_no++;
        return ReturnToken("{", LBRA);
    }
    if (LastChar == '}')
    {
        file.get(LastChar);
        column_no++;
        return ReturnToken("}", RBRA);
    }
    if (LastChar == '(')
    {
        file.get(LastChar);
        column_no++;
        return ReturnToken("(", LPAR);
    }
    if (LastChar == ')')
    {
        file.get(LastChar);
        column_no++;
        return ReturnToken(")", RPAR);
    }
    if (LastChar == ';')
    {
        file.get(LastChar);
        column_no++;
        return ReturnToken(";", SC);
    }
    if (LastChar == ',')
    {
        file.get(LastChar);
        column_no++;
        return ReturnToken(",", COMMA);
    }

    if (isdigit(LastChar) || LastChar == '.')
    { // Number: [0-9]+.
        std::string num_str;

        if (LastChar == '.')
        { // Floatingpoint Number: .[0-9]+
            do
            {
                num_str += LastChar;
                file.get(LastChar);
                column_no++;
            } while (isdigit(LastChar));

            float_val = strtof(num_str.c_str(), nullptr);
            return ReturnToken(num_str, FLOAT_LIT);
        }
        else
        {
            do
            { // Start of Number: [0-9]+
                num_str += LastChar;
                file.get(LastChar);
                column_no++;
            } while (isdigit(LastChar));

            if (LastChar == '.')
            { // Floatingpoint Number: [0-9]+.[0-9]+)
                do
                {
                    num_str += LastChar;
                    file.get(LastChar);
                    column_no++;
                } while (isdigit(LastChar));

                float_val = strtof(num_str.c_str(), nullptr);
                return ReturnToken(num_str, FLOAT_LIT);
            }
            else
            { // Integer : [0-9]+
                int_val = strtod(num_str.c_str(), nullptr);
                return ReturnToken(num_str, INT_LIT);
            }
        }
    }

    if (LastChar == '&')
    {
        file.get(NextChar);
        if (NextChar == '&')
        { // AND: &&
            file.get(LastChar);
            column_no += 2;
            return ReturnToken("&&", AND);
        }
        else
        {
            LastChar = NextChar;
            column_no++;
            return ReturnToken("&", int('&'));
        }
    }

    if (LastChar == '|')
    {
        file.get(NextChar);
        if (NextChar == '|')
        { // OR: ||
            file.get(LastChar);
            column_no += 2;
            return ReturnToken("||", OR);
        }
        else
        {
            LastChar = NextChar;
            column_no++;
            return ReturnToken("|", int('|'));
        }
    }

    if (LastChar == '!')
    {
        file.get(NextChar);
        if (NextChar == '=')
        { // NE: !=
            file.get(LastChar);
            column_no += 2;
            return ReturnToken("!=", NE);
        }
        else
        {
            LastChar = NextChar;
            column_no++;
            return ReturnToken("!", NOT);
        }
    }

    if (LastChar == '<')
    {
        file.get(NextChar);
        if (NextChar == '=')
        { // LE: <=
            file.get(LastChar);
            column_no += 2;
            return ReturnToken("<=", LE);
        }
        else
        {
            LastChar = NextChar;
            column_no++;
            return ReturnToken("<", LT);
        }
    }

    if (LastChar == '>')
    {
        file.get(NextChar);
        if (NextChar == '=')
        { // GE: >=
            file.get(LastChar);
            column_no += 2;
            return ReturnToken(">=", GE);
        }
        else
        {
            LastChar = NextChar;
            column_no++;
            return ReturnToken(">", GT);
        }
    }

    if (LastChar == '/')
    { // could be division or could be the start of a comment
        file.get(LastChar);
        column_no++;
        if (LastChar == '/')
        { // definitely a comment
            do
            {
                file.get(LastChar);
                column_no++;
            } while (file.peek() != EOF && LastChar != '\n' && LastChar != '\r');

            if (file.peek() != EOF)
                return GetToken();
        }
        else
            return ReturnToken("/", DIV);
    }

    if (file.peek() == EOF)
    {
        column_no++;
        return ReturnToken("0", EOF_TOK);
    }

    // Otherwise, just return the character as its ascii value.
    char ThisChar = LastChar;
    std::string s(1, ThisChar);
    file.get(LastChar);
    column_no++;
    return ReturnToken(s, ThisChar);
}
#include "token.h"

bool IsType(const TOKEN_TYPE tok)
{
    return (tok == INT_TOK) ||
           (tok == FLOAT_TOK) ||
           (tok == BOOL_TOK) ||
           (tok == VOID_TOK);
}

bool IsNonVoidType(const TOKEN_TYPE tok)
{
    return (tok == INT_TOK) ||
           (tok == FLOAT_TOK) ||
           (tok == BOOL_TOK);
}

std::ostream &
operator<<(std::ostream &out, const Token &tok)
{
    out << "[lexeme: '" << tok.lexeme << "', type: " << static_cast<int>(tok.type) << ", " << tok.line_no << ":" << tok.column_no << "]";
    return out;
}

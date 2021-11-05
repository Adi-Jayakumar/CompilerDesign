#include "parser.h"

Token Parser::GetNextToken()
{
    if (tok_buffer.size() == 0)
        tok_buffer.push_back(lex.GetToken());

    Token temp = tok_buffer.front();
    tok_buffer.pop_front();

    return cur = temp;
}

void Parser::PutBackToken(const Token &tok)
{
    tok_buffer.push_front(tok);
}

void Parser::ReportParseError(const std::string &msg)
{
    had_error = true;
    std::cerr << "[PARSE ERROR] Near '" << cur.lexeme << "' at " << cur.line_no << ":" << cur.column_no << std::endl
              << msg << std::endl;
    exit(1);
}

void Parser::CheckCurType(const TOKEN_TYPE tok, const std::string &msg)
{
    if (cur.type != tok)
        ReportParseError(msg);
}

static const std::unordered_map<TOKEN_TYPE, Type> TypeConverter{
    {VOID_TOK, Type::VOID},
    {INT_TOK, Type::INT},
    {BOOL_TOK, Type::BOOL},
    {FLOAT_TOK, Type::FLOAT},
};

#define PARSER_CHECK(exp, err_msg) \
    if (!(exp))                    \
    {                              \
        ReportParseError(err_msg); \
    }

std::vector<SP<Stmt>> Parser::ParseProgram()
{
    std::vector<SP<Stmt>> externs = ParseExternList();
    std::vector<SP<Stmt>> decls = ParseDeclarationList();
    externs.insert(externs.end(), decls.begin(), decls.end());
    return externs;
}

std::vector<SP<Stmt>> Parser::ParseExternList()
{
    std::vector<SP<Stmt>> res;
    while (!IsType(cur.type) && cur.type != EOF_TOK)
        res.push_back(ParseExtern());
    return res;
}

SP<Stmt> Parser::ParseExtern()
{
    CheckCurType(EXTERN, "Extern function declaration must start with the 'extern' keyword");
    Token loc = cur;

    GetNextToken();
    PARSER_CHECK(IsType(cur.type), "Must specify return type of extern function");
    Type ret = TypeConverter.at(cur.type);

    GetNextToken();
    CheckCurType(IDENT, "Must specify name of extern function");
    std::string name = cur.lexeme;

    GetNextToken();
    CheckCurType(LPAR, "Expect '(' to begin extern function arguments");
    GetNextToken();

    std::vector<std::pair<Type, std::string>> args = ParseFunctionArgs();
    CheckCurType(RPAR, "Expect ')' to end extern function arguments");
    GetNextToken();
    CheckCurType(SC, "Expect ';' to end extern function declaration");
    GetNextToken();

    return std::make_shared<ExternNode>(ret, name, args, loc);
}

std::vector<SP<Stmt>> Parser::ParseDeclarationList()
{
    std::vector<SP<Stmt>> res;
    while (cur.type != EOF_TOK)
        res.push_back(ParseDeclaration());
    return res;
}

SP<Stmt> Parser::ParseDeclaration()
{
    PARSER_CHECK(IsType(cur.type), "Declarations must begin with a type");
    Token loc = cur;
    Type type = TypeConverter.at(cur.type);

    GetNextToken();
    CheckCurType(IDENT, "Expect name after type for declarations");
    std::string name = cur.lexeme;

    GetNextToken();
    if (cur.type == SC)
    {
        PARSER_CHECK(type != Type::VOID, "Variables cannot be declared with 'void' type");
        GetNextToken();
        return std::make_shared<LocalDeclNode>(type, name, loc);
    }

    // must now be in the case of a function declaration (or error case of course)
    CheckCurType(LPAR, "Expect '(' to begin function parameters in function declaration");
    GetNextToken();

    std::vector<std::pair<Type, std::string>> args = ParseFunctionArgs();

    CheckCurType(RPAR, "Expect ')' to end function parameters in function declaration");
    GetNextToken();

    SP<Stmt> body = ParseBlock();
    return std::make_shared<FunctionDeclNode>(type, name, args, body, loc);
}

std::vector<std::pair<Type, std::string>> Parser::ParseFunctionArgs()
{
    std::vector<std::pair<Type, std::string>> res;
    while (cur.type != RPAR && cur.type != EOF_TOK)
    {
        std::pair<Type, std::string> cur_arg;
        PARSER_CHECK(IsNonVoidType(cur.type), "Invalid function parameter type '" + cur.lexeme + "'");
        cur_arg.first = TypeConverter.at(cur.type);

        GetNextToken();
        CheckCurType(IDENT, "Expect identifier for function parameter");
        cur_arg.second = cur.lexeme;

        GetNextToken();
        if (cur.type != RPAR)
            CheckCurType(COMMA, "Parameters in function declaration must be separated by commas");

        res.push_back(cur_arg);

        if (cur.type != RPAR)
            GetNextToken();
    }

    return res;
}

SP<Stmt> Parser::ParseStatement()
{
    if (cur.type == IF)
        return ParseIfStmt();
    else if (cur.type == WHILE)
        return ParseWhileStmt();
    else if (cur.type == RETURN)
        return ParseReturn();
    else if (cur.type == LBRA)
        return ParseBlock();
    return ParseExprStmt();
}

std::vector<SP<Stmt>> Parser::ParseStatementList(const TOKEN_TYPE end)
{
    std::vector<SP<Stmt>> res;
    while (cur.type != end && cur.type != EOF_TOK)
        res.push_back(ParseStatement());
    return res;
}

SP<Stmt> Parser::ParseExprStmt()
{
    Token loc = cur;
    SP<Expr> exp = ParseExpression();
    CheckCurType(SC, "Missing ';' to end expression statement");
    GetNextToken();
    return std::make_shared<ExprStmtNode>(exp, loc);
}

SP<Stmt> Parser::ParseIfStmt()
{
    CheckCurType(IF, "Expect 'if' at the beginning of an if-statement");
    Token loc = cur;

    GetNextToken();
    CheckCurType(LPAR, "If-statement condition must be opened by open parenthesis");
    GetNextToken();

    SP<Expr> condition = ParseExpression();

    CheckCurType(RPAR, "If-statement condition must be closed off with closing parenthesis");
    GetNextToken();

    SP<Stmt> then = ParseBlock();
    SP<Stmt> other = nullptr;

    if (cur.type == ELSE)
    {
        GetNextToken();
        other = ParseBlock();
    }

    return std::make_shared<IfElseNode>(condition, then, other, loc);
}

SP<Stmt> Parser::ParseWhileStmt()
{
    CheckCurType(WHILE, "Expect 'while' at the beginning of a while-statement");
    Token loc = cur;

    GetNextToken();
    CheckCurType(LPAR, "While-statement condition must be opened by open parenthesis");
    GetNextToken();

    SP<Expr> condition = ParseExpression();

    CheckCurType(RPAR, "While-statement condition must be closed off with closing parenthesis");
    GetNextToken();

    SP<Stmt> body = ParseStatement();
    return std::make_shared<WhileNode>(condition, body, loc);
}

SP<Stmt> Parser::ParseReturn()
{
    CheckCurType(RETURN, "Expect 'return' at the beginning of a return-statement");
    Token loc = cur;

    GetNextToken();
    SP<Expr> ret = nullptr;

    if (cur.type != SC)
        ret = ParseExpression();

    CheckCurType(SC, "Return statements must be terminated with ';'");
    GetNextToken();
    return std::make_shared<ReturnNode>(ret, loc);
}

SP<Stmt> Parser::ParseBlock()
{
    CheckCurType(LBRA, "BlockNode must start with '{'");
    Token loc = cur;
    GetNextToken();
    std::vector<SP<Stmt>> local_decls = ParseLocalDeclList();
    std::vector<SP<Stmt>> stmts = ParseStatementList(RBRA);
    CheckCurType(RBRA, "Block must end with '}'");
    GetNextToken();
    return std::make_shared<BlockNode>(local_decls, stmts, loc);
}

SP<Stmt> Parser::ParseLocalDecl()
{
    if (cur.type == VOID_TOK || cur.type == INT_TOK || cur.type == BOOL_TOK || cur.type == FLOAT_TOK)
    {
        Token loc = cur;
        Type type = TypeConverter.at(cur.type);
        GetNextToken();
        CheckCurType(IDENT, "Expect identifer after type in local variable declaration");
        std::string name = cur.lexeme;
        GetNextToken();
        CheckCurType(SC, "Expect semi-colon at the end of local variable declaration");
        GetNextToken();
        return std::make_shared<LocalDeclNode>(type, name, loc);
    }
    ReportParseError("Invalid token for variable declaration");
    // dummy return - is never reached
    return nullptr;
}

std::vector<SP<Stmt>> Parser::ParseLocalDeclList()
{
    std::vector<SP<Stmt>> res;
    while (TypeConverter.find(cur.type) != TypeConverter.end() && cur.type != EOF_TOK)
        res.push_back(ParseLocalDecl());
    return res;
}

SP<Expr> Parser::ParseExpression()
{
    SP<Expr> target = ParseOrOr();

    if (cur.type == ASSIGN)
    {
        IdentifierNode *id = dynamic_cast<IdentifierNode *>(target.get());
        if (id == nullptr)
            ReportParseError("Invalid assignment target");

        Token loc = cur;
        GetNextToken();
        SP<Expr> val = ParseExpression();
        return std::make_shared<AssignNode>(id->GetName(), loc, val);
    }

    return target;
}

SP<Expr> Parser::ParseRval()
{
    return ParseOrOr();
}

SP<Expr> Parser::ParseOrOr()
{
    SP<Expr> left = ParseAndAnd();
    while (cur.type == OR)
    {
        Token op = cur;
        GetNextToken();
        SP<Expr> right = ParseAndAnd();
        left = std::make_shared<BinaryNode>(left, op, right);
    }
    return left;
}

SP<Expr> Parser::ParseAndAnd()
{
    SP<Expr> left = ParseEquality();

    while (cur.type == AND)
    {
        Token op = cur;
        GetNextToken();
        SP<Expr> right = ParseEquality();
        left = std::make_shared<BinaryNode>(left, op, right);
    }
    return left;
}

SP<Expr> Parser::ParseEquality()
{
    SP<Expr> left = ParseComparison();

    while (cur.type == EQ || cur.type == NE)
    {
        Token op = cur;
        GetNextToken();
        SP<Expr> right = ParseComparison();
        left = std::make_shared<BinaryNode>(left, op, right);
    }
    return left;
}

SP<Expr> Parser::ParseComparison()
{
    SP<Expr> left = ParseAddSub();

    while (cur.type == GT || cur.type == GE || cur.type == LT || cur.type == LE)
    {
        Token op = cur;
        GetNextToken();
        SP<Expr> right = ParseAddSub();
        left = std::make_shared<BinaryNode>(left, op, right);
    }
    return left;
}

SP<Expr> Parser::ParseAddSub()
{
    SP<Expr> left = ParseMulDivMod();

    while (cur.type == PLUS || cur.type == MINUS)
    {
        Token op = cur;
        GetNextToken();
        SP<Expr> right = ParseMulDivMod();
        left = std::make_shared<BinaryNode>(left, op, right);
    }
    return left;
}

SP<Expr> Parser::ParseMulDivMod()
{
    SP<Expr> left = ParseUnarySubNot();

    while (cur.type == ASTERIX || cur.type == DIV || cur.type == MOD)
    {
        Token op = cur;
        GetNextToken();
        SP<Expr> right = ParseUnarySubNot();
        left = std::make_shared<BinaryNode>(left, op, right);
    }
    return left;
}

SP<Expr> Parser::ParseUnarySubNot()
{
    if (cur.type == MINUS || cur.type == NOT)
    {
        Token op = cur;
        GetNextToken();
        return std::make_shared<UnaryNode>(op, ParseUnarySubNot());
    }

    return ParseParen();
}

SP<Expr> Parser::ParseParen()
{
    if (cur.type == LPAR)
    {
        GetNextToken();
        SP<Expr> exp = ParseExpression();
        CheckCurType(RPAR, "Expected ')'");
        GetNextToken();
        return exp;
    }
    return ParseIdent();
}

SP<Expr> Parser::ParseIdent()
{
    if (cur.type == IDENT)
    {
        Token id = cur;
        GetNextToken();
        if (cur.type == LPAR)
        {
            GetNextToken();
            std::vector<SP<Expr>> args = ParseArgs();
            CheckCurType(RPAR, "Expected ')' to close function call");
            GetNextToken();
            return std::make_shared<FunctionCallNode>(id, args);
        }
        return std::make_shared<IdentifierNode>(id);
    }
    return ParseLiteral();
}

std::vector<SP<Expr>> Parser::ParseArgs()
{
    std::vector<SP<Expr>> res;
    while (cur.type != RPAR && cur.type != EOF_TOK)
    {
        res.push_back(ParseExpression());
        if (cur.type != RPAR && cur.type != EOF_TOK)
        {
            CheckCurType(COMMA, "Arguments in a function call must be seperated by commas");
            GetNextToken();
        }
    }
    return res;
}

SP<Expr> Parser::ParseLiteral()
{
    if (cur.type == INT_LIT)
    {
        Token lit = cur;
        int val = lex.int_val;
        GetNextToken();
        return std::make_shared<IntNode>(val, lit);
    }
    else if (cur.type == FLOAT_LIT)
    {
        Token lit = cur;
        float val = lex.float_val;
        GetNextToken();
        return std::make_shared<FloatNode>(val, lit);
    }
    else if (cur.type == BOOL_LIT)
    {
        Token lit = cur;
        bool val = lex.bool_val;
        GetNextToken();
        return std::make_shared<BoolNode>(val, lit);
    }
    else
        ReportParseError("Invalid token '" + cur.lexeme + "'");
    // should never be reached
    return nullptr;
}
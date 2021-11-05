#pragma once
#include "astnodes.h"
#include "lexer.h"
#include <deque>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

class Parser
{
    Token cur;
    std::deque<Token> tok_buffer;
    Lexer lex;
    bool had_error;

public:
    Parser(std::ifstream &_file) : lex(_file), had_error(false) { GetNextToken(); };

    Token GetNextToken();
    void PutBackToken(const Token &tok);

    void ReportParseError(const std::string &msg);
    void CheckCurType(const TOKEN_TYPE type, const std::string &msg);

    std::vector<SP<Stmt>> ParseProgram();

    std::vector<SP<Stmt>> ParseExternList();
    SP<Stmt> ParseExtern();

    std::vector<SP<Stmt>> ParseDeclarationList();
    SP<Stmt> ParseDeclaration();
    // TODO - Handle 'int f(void);' case;
    std::vector<std::pair<Type, std::string>> ParseFunctionArgs();

    SP<Stmt> ParseStatement();
    std::vector<SP<Stmt>> ParseStatementList(const TOKEN_TYPE end);
    SP<Stmt> ParseExprStmt();
    SP<Stmt> ParseIfStmt();
    SP<Stmt> ParseWhileStmt();
    SP<Stmt> ParseReturn();

    SP<Stmt> ParseBlock();
    SP<Stmt> ParseLocalDecl();
    std::vector<SP<Stmt>> ParseLocalDeclList();

    SP<Expr> ParseExpression();
    SP<Expr> ParseRval();
    // methods called by 'ParseRval' to enforce operator precedence
    SP<Expr> ParseOrOr();
    SP<Expr> ParseAndAnd();
    SP<Expr> ParseEquality();
    SP<Expr> ParseComparison();
    SP<Expr> ParseAddSub();
    SP<Expr> ParseMulDivMod();
    SP<Expr> ParseUnarySubNot();
    SP<Expr> ParseParen();
    SP<Expr> ParseIdent();
    std::vector<SP<Expr>> ParseArgs();
    SP<Expr> ParseLiteral();
};

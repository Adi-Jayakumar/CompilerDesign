#pragma once
#include "astnodes.h"
#include "symboltable.h"

class Compiler
{
    SymbolTable symbols;

public:
    Compiler() = default;

    void CompileProgram(std::vector<SP<Stmt>> &program);
    void ReportSemanticError(const std::string &msg, const Token loc) const;

    // Expressions
    Type CompileIntNode(IntNode *);
    Type CompileFloatNode(FloatNode *);
    Type CompileBoolNode(BoolNode *);
    Type CompileUnaryNode(UnaryNode *);
    Type CompileBinaryNode(BinaryNode *);
    Type CompileIdentifierNode(IdentifierNode *);
    Type CompileAssignNode(AssignNode *);
    Type CompileFunctionCallNode(FunctionCallNode *);
    Type CompileCoercionNode(CoercionNode *);

    // Statements
    void CompileExprStmtNode(ExprStmtNode *);
    void CompileWhileNode(WhileNode *);
    void CompileIfElseNode(IfElseNode *);
    void CompileReturnNode(ReturnNode *);
    void CompileLocalDeclNode(LocalDeclNode *);
    void CompileBlockNode(BlockNode *);
    void CompileFunctionDeclNode(FunctionDeclNode *);
    void CompileExternNode(ExternNode *);
};
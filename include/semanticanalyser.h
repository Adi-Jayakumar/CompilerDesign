#pragma once
#include "astnodes.h"
#include "symboltable.h"

class SemanticAnalyser
{
    SymbolTable symbols;
    FunctionDeclNode *cur_func;

public:
    SemanticAnalyser() = default;

    void AnalyseProgram(std::vector<SP<Stmt>> &program);
    void ReportSemanticError(const std::string &msg, const Token loc) const;

    // Expressions
    Type AnalyseIntNode(IntNode *);
    Type AnalyseFloatNode(FloatNode *);
    Type AnalyseBoolNode(BoolNode *);
    Type AnalyseUnaryNode(UnaryNode *);
    Type AnalyseBinaryNode(BinaryNode *);
    Type AnalyseIdentifierNode(IdentifierNode *);
    Type AnalyseAssignNode(AssignNode *);
    Type AnalyseFunctionCallNode(FunctionCallNode *);
    Type AnalyseCoercionNode(CoercionNode *);

    // Statements
    void AnalyseExprStmtNode(ExprStmtNode *);
    void AnalyseWhileNode(WhileNode *);
    void AnalyseIfElseNode(IfElseNode *);
    void AnalyseReturnNode(ReturnNode *);
    void AnalyseLocalDeclNode(LocalDeclNode *);
    void AnalyseBlockNode(BlockNode *);
    void AnalyseFunctionDeclNode(FunctionDeclNode *);
    void AnalyseExternNode(ExternNode *);
};
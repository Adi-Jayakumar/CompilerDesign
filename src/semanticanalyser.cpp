#include "semanticanalyser.h"

void SemanticAnalyser::AnalyseProgram(std::vector<SP<Stmt>> &program)
{
    for (auto &stmt : program)
        stmt->Analyse(*this);
}

void SemanticAnalyser::ReportSemanticError(const std::string &msg, const Token loc) const
{
    std::cerr << "[SEMANTIC ERROR] Near '" << loc.lexeme << "' at " << loc.line_no << ':' << loc.column_no << std::endl
              << msg << std::endl;
    exit(1);
}

//------------------------------------------EXPRESSIONS------------------------------------------//

Type SemanticAnalyser::AnalyseIntNode(IntNode *i)
{
    return i->type = Type::INT;
}

Type SemanticAnalyser::AnalyseFloatNode(FloatNode *f)
{
    return f->type = Type::FLOAT;
}

Type SemanticAnalyser::AnalyseBoolNode(BoolNode *b)
{
    return b->type = Type::BOOL;
}

Type SemanticAnalyser::AnalyseUnaryNode(UnaryNode *u)
{
    Type right = u->right->Analyse(*this);
    std::optional<Type> type = symbols.CheckUnaryOperatorUse(u->loc.type, right);
    if (!type)
        ReportSemanticError("bad unary", u->loc);
    return u->type = type.value();
}

Type SemanticAnalyser::AnalyseBinaryNode(BinaryNode *b)
{
    Type left = b->left->Analyse(*this);
    Type right = b->right->Analyse(*this);
    std::optional<Type> type = symbols.CheckBinaryOperatorUse(left, b->loc.type, right);

    if (!type)
        ReportSemanticError("bad binary", b->loc);

    // type 'float' is infectious, if one argument of a binary operator is a float
    // and the other is not, both must be converted to float before executing the
    // operation

    if (left == Type::FLOAT && right != Type::FLOAT)
    {
        b->right = std::make_shared<CoercionNode>(Type::FLOAT, b->right, b->right->loc);
        b->right->Analyse(*this);
    }
    else if (left != Type::FLOAT && right == Type::FLOAT)
    {
        b->left = std::make_shared<CoercionNode>(Type::FLOAT, b->left, b->left->loc);
        b->left->Analyse(*this);
    }

    return b->type = type.value();
}

Type SemanticAnalyser::AnalyseIdentifierNode(IdentifierNode *i)
{
    std::optional<VarID> var = symbols.ResolveVariable(i->GetName());
    if (!var)
        ReportSemanticError("undefined variavle", i->loc);
    return i->type = var.value().type;
}

Type SemanticAnalyser::AnalyseAssignNode(AssignNode *a)
{
    std::optional<VarID> v_targ = symbols.ResolveVariable(a->name);

    if (!v_targ)
        ReportSemanticError("bad assign target", a->loc);

    Type target = v_targ.value().type;
    Type value = a->val->Analyse(*this);

    if (!symbols.CanAssign(target, value))
        ReportSemanticError("bad assign types", a->loc);

    if (target != value)
    {
        a->val = std::make_shared<CoercionNode>(target, a->val, a->val->loc);
        a->val->Analyse(*this);
    }

    return a->type = target;
}

Type SemanticAnalyser::AnalyseFunctionCallNode(FunctionCallNode *fc)
{
    std::vector<Type> argtypes;

    for (auto &arg : fc->args)
        argtypes.push_back(arg->Analyse(*this));

    std::optional<FuncID> func = symbols.ResolveFunction(fc->loc.lexeme, argtypes);

    if (!func)
        ReportSemanticError("bad function call", fc->loc);

    // SymbolTable should have ensured that the FunctionCallNode and the returned FuncID
    // have the same number of arguments so this is fine

    std::vector<Type> actual_argtypes = func.value().args;

    for (size_t i = 0; i < fc->args.size(); i++)
    {
        if (actual_argtypes[i] != argtypes[i])
        {
            fc->args[i] = std::make_shared<CoercionNode>(actual_argtypes[i], fc->args[i], fc->args[i]->loc);
            fc->args[i]->Analyse(*this);
        }
    }

    return fc->type = func.value().ret;
}

Type SemanticAnalyser::AnalyseCoercionNode(CoercionNode *c)
{
    // should never be triggered
    assert(symbols.CanAssign(c->type, c->exp->type));
    return c->type;
}

//------------------------------------------STATEMENTS------------------------------------------//

void SemanticAnalyser::AnalyseExprStmtNode(ExprStmtNode *es)
{
    es->exp->Analyse(*this);
}

void SemanticAnalyser::AnalyseWhileNode(WhileNode *w)
{
    Type cond = w->condition->Analyse(*this);

    if (cond != Type::BOOL && cond != Type::INT)
        ReportSemanticError("bad while condition", w->loc);

    w->body->Analyse(*this);
}

void SemanticAnalyser::AnalyseIfElseNode(IfElseNode *ie)
{
    Type cond = ie->condition->Analyse(*this);

    if (cond != Type::BOOL && cond != Type::INT)
        ReportSemanticError("bad if condition", ie->loc);

    ie->then->Analyse(*this);
    if (ie->other != nullptr)
        ie->other->Analyse(*this);
}

void SemanticAnalyser::AnalyseReturnNode(ReturnNode *r)
{
    r->ret->Analyse(*this);
    // TODO - Check return type matches current function
}

void SemanticAnalyser::AnalyseLocalDeclNode(LocalDeclNode *ld)
{
    if (!symbols.AddVariable(ld->type, ld->name))
        ReportSemanticError("bad local decl", ld->loc);
}

void SemanticAnalyser::AnalyseBlockNode(BlockNode *b)
{
    for (auto &decl : b->local_decls)
        decl->Analyse(*this);
    for (auto &stmt : b->stmts)
        stmt->Analyse(*this);
}

void SemanticAnalyser::AnalyseFunctionDeclNode(FunctionDeclNode *fd)
{
    std::vector<Type> argtypes;
    for (const auto &arg : fd->args)
        argtypes.push_back(arg.first);

    if (!symbols.AddFunction(fd->ret, fd->name, argtypes))
        ReportSemanticError("bad func decl", fd->loc);

    ++symbols.depth;

    for (const auto &arg : fd->args)
        symbols.AddVariable(arg.first, arg.second);

    fd->body->Analyse(*this);

    symbols.ClearCurrentScope();
    --symbols.depth;
}

void SemanticAnalyser::AnalyseExternNode(ExternNode *e)
{
    std::vector<Type> argtypes;
    for (const auto &arg : e->args)
        argtypes.push_back(arg.first);

    if (!symbols.AddFunction(e->ret, e->name, argtypes))
        ReportSemanticError("bad extern decl", e->loc);
}

//------------------------------------------EXPRESSIONS------------------------------------------//

Type IntNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseIntNode(this);
}

Type FloatNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseFloatNode(this);
}

Type BoolNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseBoolNode(this);
}

Type UnaryNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseUnaryNode(this);
}

Type BinaryNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseBinaryNode(this);
}

Type IdentifierNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseIdentifierNode(this);
}

Type AssignNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseAssignNode(this);
}

Type FunctionCallNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseFunctionCallNode(this);
}

Type CoercionNode::Analyse(SemanticAnalyser &sa)
{
    return sa.AnalyseCoercionNode(this);
}

//------------------------------------------STATEMENTS------------------------------------------//

void ExprStmtNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseExprStmtNode(this);
}

void WhileNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseWhileNode(this);
}

void IfElseNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseIfElseNode(this);
}

void ReturnNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseReturnNode(this);
}

void LocalDeclNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseLocalDeclNode(this);
}

void BlockNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseBlockNode(this);
}

void FunctionDeclNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseFunctionDeclNode(this);
}

void ExternNode::Analyse(SemanticAnalyser &sa)
{
    sa.AnalyseExternNode(this);
}

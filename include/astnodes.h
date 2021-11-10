#pragma once
#include "token.h"
#include "type.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include "llvm/IR/Module.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

// Forward declarations of SemanticAnalyser and Compiler classes
class SemanticAnalyser;
class Compiler;

/// Expr - Base struct for all expression nodes.
struct Expr
{
    Type type;
    Token loc;
    Expr(const Type _type, const Token &_loc) : type(_type), loc(_loc){};
    Expr(const Token &_loc) : loc(_loc){};

    virtual ~Expr() {}

    virtual std::string to_string() const = 0;

    // implemented in "semanticanalyser.cpp" for all derived classes
    virtual Type Analyse(SemanticAnalyser &) = 0;
    // implemented in "compiler.cpp" for all derived classes
    virtual llvm::Value *Compile(Compiler &) = 0;
};

struct Stmt
{
    Token loc;
    Stmt(const Token &_loc) : loc(_loc){};
    virtual ~Stmt() {}

    virtual std::string to_string() const = 0;

    // implemented in "semanticanalyser.cpp" for all derived classes
    virtual void Analyse(SemanticAnalyser &) = 0;
    // implemented in "compiler.cpp" for all derived classes
    virtual llvm::Value *Compile(Compiler &) = 0;
};

template <typename T>
using SP = std::shared_ptr<T>;

//------------------------------------------EXPRESSIONS------------------------------------------//
struct IntNode : Expr
{
    int val;

    IntNode(const int _val,
            const Token &_loc) : Expr(_loc),
                                 val(_val){};

    virtual std::string to_string() const override
    {
        return std::to_string(val);
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct FloatNode : Expr
{
    double val;

    FloatNode(const double _val,
              const Token &_loc) : Expr(_loc),
                                   val(_val){};

    virtual std::string to_string() const override
    {
        return std::to_string(val);
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct BoolNode : Expr
{
    bool val;

    BoolNode(const bool _val,
             const Token &_loc) : Expr(_loc),
                                  val(_val){};

    virtual std::string to_string() const override
    {
        return val ? "true" : "false";
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct UnaryNode : Expr
{
    SP<Expr> right;

    UnaryNode(const Token &_op,
              const SP<Expr> &_right) : Expr(_op),
                                        right(_right){};

    virtual std::string to_string() const override
    {
        return loc.lexeme + "(" + right->to_string() + ")";
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct BinaryNode : Expr
{
    SP<Expr> left;
    SP<Expr> right;

    BinaryNode(const SP<Expr> &_left,
               const Token &_op,
               const SP<Expr> &_right) : Expr(_op),
                                         left(_left),
                                         right(_right){};

    virtual std::string to_string() const override
    {
        return "(" + left->to_string() + " " + loc.lexeme + " " + right->to_string() + ")";
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct IdentifierNode : Expr
{

    IdentifierNode(const Token &_id) : Expr(_id){};

    std::string GetName() { return loc.lexeme; };

    virtual std::string to_string() const override
    {
        return loc.lexeme;
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct AssignNode : Expr
{
    std::string name;
    SP<Expr> val;

    AssignNode(const std::string &_name,
               const Token &_loc,
               const SP<Expr> &_val) : Expr(_loc),
                                       name(_name),
                                       val(_val){};

    virtual std::string to_string() const override
    {
        return "(" + name + " = " + val->to_string() + ")";
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct FunctionCallNode : Expr
{
    std::vector<SP<Expr>> args;

    FunctionCallNode(const Token &_id,
                     const std::vector<SP<Expr>> &_args) : Expr(_id),
                                                           args(_args){};

    virtual std::string to_string() const override
    {
        std::string res = loc.lexeme + "(";
        if (args.size() > 0)
        {
            for (size_t i = 0; i < args.size() - 1; i++)
                res += args[i]->to_string() + ", ";
            res += args.back()->to_string() + ")";
        }
        else
            res += ")";
        return res;
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

struct CoercionNode : Expr
{
    SP<Expr> exp;

    CoercionNode(const Type _new_type, const SP<Expr> &_exp, const Token &_loc) : Expr(_new_type, _loc), exp(_exp){};

    virtual std::string to_string() const override
    {
        return ExprType::to_string(type) + "(" + exp->to_string() + ")";
    };

    virtual Type Analyse(SemanticAnalyser &) override;
    virtual llvm::Value* Compile(Compiler &) override;
};

//------------------------------------------STATEMENTS------------------------------------------//
struct ExprStmtNode : Stmt
{
    SP<Expr> exp;

    ExprStmtNode(const SP<Expr> &_exp,
                 const Token &_loc) : Stmt(_loc),
                                      exp(_exp){};

    std::string to_string() const override
    {
        return exp->to_string() + ";";
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};

struct WhileNode : Stmt
{
    SP<Expr> condition;
    SP<Stmt> body;

    WhileNode(const SP<Expr> &_condition,
              const SP<Stmt> &_body,
              const Token &_loc) : Stmt(_loc),
                                   condition(_condition),
                                   body(_body){};

    std::string to_string() const override
    {
        return "while (" + condition->to_string() + ")\n" + body->to_string();
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};

struct IfElseNode : Stmt
{
    SP<Expr> condition;
    SP<Stmt> then, other;

    IfElseNode(const SP<Expr> &_condition,
               const SP<Stmt> &_then,
               const SP<Stmt> _other,
               const Token &_loc) : Stmt(_loc),
                                    condition(_condition),
                                    then(_then),
                                    other(_other){};

    std::string to_string() const override
    {
        std::string res = "if (" + condition->to_string() + ")\n" + then->to_string();
        if (other != nullptr)
            res += "else\n" + other->to_string();
        return res;
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};

struct ReturnNode : Stmt
{
    SP<Expr> ret;

    ReturnNode(const SP<Expr> &_ret,
               const Token &_loc) : Stmt(_loc),
                                    ret(_ret){};

    std::string to_string() const override
    {
        std::string res = "return";
        if (ret != nullptr)
            res += " " + ret->to_string();
        res += ";";
        return res;
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};

struct LocalDeclNode : Stmt
{
    Type type;
    std::string name;

    LocalDeclNode(const Type _type,
                  const std::string &_name,
                  const Token &_loc) : Stmt(_loc),
                                       type(_type),
                                       name(_name){};

    std::string to_string() const override
    {
        return ExprType::to_string(type) + " " + name + ";";
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};

struct BlockNode : Stmt
{
    std::vector<SP<Stmt>> local_decls;
    std::vector<SP<Stmt>> stmts;

    BlockNode(const std::vector<SP<Stmt>> &_local_decls,
              const std::vector<SP<Stmt>> &_stmts,
              const Token &_loc) : Stmt(_loc),
                                   local_decls(_local_decls),
                                   stmts(_stmts){};

    std::string to_string() const override
    {
        std::string res = "{\n";
        for (const auto &decl : local_decls)
            res += decl->to_string() + "\n";
        for (const auto &stmt : stmts)
            res += stmt->to_string() + "\n";
        res += "}";
        return res;
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};

struct FunctionDeclNode : Stmt
{
    Type ret;
    std::string name;
    std::vector<std::pair<Type, std::string>> args;
    SP<Stmt> body;

    FunctionDeclNode(const Type _ret,
                     const std::string &_name,
                     const std::vector<std::pair<Type, std::string>> &_args,
                     const SP<Stmt> &_body,
                     const Token &_loc) : Stmt(_loc),
                                          ret(_ret),
                                          name(_name),
                                          args(_args),
                                          body(_body){};

    std::string to_string() const override
    {
        std::string res = ExprType::to_string(ret) + " " + name + "(";

        if (args.size() > 0)
        {
            for (size_t i = 0; i < args.size() - 1; i++)
                res += ExprType::to_string(args[i].first) + " " + args[i].second + ", ";
            res += ExprType::to_string(args.back().first) + " " + args.back().second;
        }

        res += ")\n" + body->to_string();
        return res;
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};

struct ExternNode : Stmt
{
    Type ret;
    std::string name;
    std::vector<std::pair<Type, std::string>> args;

    ExternNode(const Type _ret,
               const std::string &_name,
               const std::vector<std::pair<Type, std::string>> &_args,
               const Token &_loc) : Stmt(_loc),
                                    ret(_ret),
                                    name(_name),
                                    args(_args){};

    std::string to_string() const override
    {
        std::string res = "extern " + ExprType::to_string(ret) + " " + name + "(";
        size_t num_args = args.size();

        if (num_args > 1)
        {
            for (size_t i = 0; i < num_args; i++)
                res += ExprType::to_string(args[i].first) + " " + args[i].second + ", ";
        }
        --num_args;
        res += ExprType::to_string(args[num_args].first) + " " + args[num_args].second + ");";
        return res;
    };

    virtual void Analyse(SemanticAnalyser &) override;
    virtual llvm::Value *Compile(Compiler &) override;
};
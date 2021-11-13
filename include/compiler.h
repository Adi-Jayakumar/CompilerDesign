#pragma once
#include "astnodes.h"
#include "symboltable.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

class Compiler
{
    const char *out_file = "output.ll";
    // SymbolTable symbols;

    llvm::LLVMContext llvm_context;
    llvm::IRBuilder<> llvm_builder;
    std::unique_ptr<llvm::Module> llvm_module;
    llvm::Function *cur_func;

    llvm::Value *S_INT32_ZERO, *F32_ZERO, *BOOL_TRUE, *BOOL_FALSE;

    llvm::Type *TypeToLLVMType(const Type type);
    llvm::AllocaInst *CreateAlloca(const Type var_type, const std::string &name);

    std::unordered_map<std::string, llvm::AllocaInst *> named_values;

public:
    Compiler() : llvm_builder(llvm_context)
    {
        llvm_module = std::make_unique<llvm::Module>("mini-c", llvm_context);
        S_INT32_ZERO = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm_context), 0, true);
        F32_ZERO = llvm::ConstantFP::get(llvm_context, llvm::APFloat(0.0));
        BOOL_TRUE = llvm::ConstantInt::get(llvm::Type::getInt1Ty(llvm_context), 1, false);
        BOOL_FALSE = llvm::ConstantInt::get(llvm::Type::getInt1Ty(llvm_context), 0, false);
    };

    void PrintCode()
    {
        llvm_module->print(llvm::errs(), nullptr);
    };

    void WriteCode()
    {
        std::error_code error_code;
        llvm::raw_fd_ostream dest(out_file, error_code, llvm::sys::fs::F_None);

        if (error_code)
        {
            llvm::errs() << "Could not open file: " << error_code.message();
            exit(1);
        }

        llvm_module->print(dest, nullptr);
    };

    void CompileProgram(std::vector<SP<Stmt>> &program);
    void ReportSemanticError(const std::string &msg, const Token loc) const;

    // Expressions
    llvm::Value *CompileIntNode(IntNode *);
    llvm::Value *CompileFloatNode(FloatNode *);
    llvm::Value *CompileBoolNode(BoolNode *);
    llvm::Value *CompileUnaryNode(UnaryNode *);
    llvm::Value *CompileBinaryNode(BinaryNode *);
    llvm::Value *CompileIdentifierNode(IdentifierNode *);
    llvm::Value *CompileAssignNode(AssignNode *);
    llvm::Value *CompileFunctionCallNode(FunctionCallNode *);
    llvm::Value *CompileCoercionNode(CoercionNode *);

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
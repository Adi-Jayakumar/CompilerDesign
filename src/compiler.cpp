#include "compiler.h"

//------------------------------------------EXPRESSIONS------------------------------------------//

llvm::Value *Compiler::CompileIntNode(IntNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileFloatNode(FloatNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileBoolNode(BoolNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileUnaryNode(UnaryNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileBinaryNode(BinaryNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileIdentifierNode(IdentifierNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileAssignNode(AssignNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileFunctionCallNode(FunctionCallNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileCoercionNode(CoercionNode *)
{
    return nullptr;
}

//------------------------------------------STATEMENTS------------------------------------------//

llvm::Value *Compiler::CompileExprStmtNode(ExprStmtNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileWhileNode(WhileNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileIfElseNode(IfElseNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileReturnNode(ReturnNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileLocalDeclNode(LocalDeclNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileBlockNode(BlockNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileFunctionDeclNode(FunctionDeclNode *)
{
    return nullptr;
}

llvm::Value *Compiler::CompileExternNode(ExternNode *)
{
    return nullptr;
}

//------------------------------------------EXPRESSIONS------------------------------------------//

llvm::Value *IntNode::Compile(Compiler &c)
{
    return c.CompileIntNode(this);
}

llvm::Value *FloatNode::Compile(Compiler &c)
{
    return c.CompileFloatNode(this);
}

llvm::Value *BoolNode::Compile(Compiler &c)
{
    return c.CompileBoolNode(this);
}

llvm::Value *UnaryNode::Compile(Compiler &c)
{
    return c.CompileUnaryNode(this);
}

llvm::Value *BinaryNode::Compile(Compiler &c)
{
    return c.CompileBinaryNode(this);
}

llvm::Value *IdentifierNode::Compile(Compiler &c)
{
    return c.CompileIdentifierNode(this);
}

llvm::Value *AssignNode::Compile(Compiler &c)
{
    return c.CompileAssignNode(this);
}

llvm::Value *FunctionCallNode::Compile(Compiler &c)
{
    return c.CompileFunctionCallNode(this);
}

llvm::Value *CoercionNode::Compile(Compiler &c)
{
    return c.CompileCoercionNode(this);
}

//------------------------------------------STATEMENTS------------------------------------------//

llvm::Value *ExprStmtNode::Compile(Compiler &c)
{
    return c.CompileExprStmtNode(this);
}

llvm::Value *WhileNode::Compile(Compiler &c)
{
    return c.CompileWhileNode(this);
}

llvm::Value *IfElseNode::Compile(Compiler &c)
{
    return c.CompileIfElseNode(this);
}

llvm::Value *ReturnNode::Compile(Compiler &c)
{
    return c.CompileReturnNode(this);
}

llvm::Value *LocalDeclNode::Compile(Compiler &c)
{
    return c.CompileLocalDeclNode(this);
}

llvm::Value *BlockNode::Compile(Compiler &c)
{
    return c.CompileBlockNode(this);
}

llvm::Value *FunctionDeclNode::Compile(Compiler &c)
{
    return c.CompileFunctionDeclNode(this);
}

llvm::Value *ExternNode::Compile(Compiler &c)
{
    return c.CompileExternNode(this);
}

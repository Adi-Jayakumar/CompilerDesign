#include "compiler.h"

//------------------------------------------EXPRESSIONS------------------------------------------//

llvm::Value *Compiler::CompileIntNode(IntNode *i)
{
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm_context), i->val, true);
}

llvm::Value *Compiler::CompileFloatNode(FloatNode *f)
{
    return llvm::ConstantFP::get(llvm_context, llvm::APFloat(float(f->val)));
}

llvm::Value *Compiler::CompileBoolNode(BoolNode *b)
{
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(llvm_context), b->val, false);
}

llvm::Value *Compiler::CompileUnaryNode(UnaryNode *u)
{
    llvm::Value *right = u->right->Compile(*this);

    if (u->loc.type == MINUS)
    {
        if (u->type == Type::INT)
            return llvm_builder.CreateSub(S_INT32_ZERO, right, "i_unary_neg");
        if (u->type == Type::FLOAT)
            return llvm_builder.CreateFSub(F32_ZERO, right, "f_unary_neg");
    }
    if (u->loc.type == NOT)
    {
        if (u->type == Type::BOOL)
            return llvm_builder.CreateNot(right, "b_not");
    }
    return nullptr;
}

llvm::Value *Compiler::CompileBinaryNode(BinaryNode *b)
{
    llvm::Value *left = b->left->Compile(*this);
    llvm::Value *right = b->right->Compile(*this);

    if (b->loc.type == OR)
        return llvm_builder.CreateOr(left, right, "bool_or");
    else if (b->loc.type == AND)
        return llvm_builder.CreateAnd(left, right, "bool_and");
    else if (b->loc.type == EQ)
    {
        if (b->left->type == Type::INT)
            return llvm_builder.CreateICmpEQ(left, right, "int_eq");
        if (b->left->type == Type::FLOAT)
            return llvm_builder.CreateFCmpUEQ(left, right, "float_eq");
        if (b->left->type == Type::BOOL)
            return llvm_builder.CreateICmpEQ(left, right, "bool_eq");
    }
    else if (b->loc.type == NE)
    {
        if (b->left->type == Type::INT)
            return llvm_builder.CreateICmpNE(left, right, "int_neq");
        if (b->left->type == Type::FLOAT)
            return llvm_builder.CreateFCmpUNE(left, right, "float_neq");
        if (b->left->type == Type::BOOL)
            return llvm_builder.CreateICmpNE(left, right, "bool_neq");
    }
    else if (b->loc.type == LE)
    {
        if (b->left->type == Type::INT)
            return llvm_builder.CreateICmpSLE(left, right, "int_leq");
        if (b->left->type == Type::FLOAT)
            return llvm_builder.CreateFCmpULE(left, right, "float_leq");
    }
    else if (b->loc.type == LT)
    {
        if (b->left->type == Type::INT)
            return llvm_builder.CreateICmpSLT(left, right, "int_lt");
        if (b->left->type == Type::FLOAT)
            return llvm_builder.CreateFCmpULT(left, right, "float_lt");
    }
    else if (b->loc.type == GE)
    {
        if (b->left->type == Type::INT)
            return llvm_builder.CreateICmpSGE(left, right, "int_geq");
        if (b->left->type == Type::FLOAT)
            return llvm_builder.CreateFCmpUGE(left, right, "float_geq");
    }
    else if (b->loc.type == GT)
    {
        if (b->left->type == Type::INT)
            return llvm_builder.CreateICmpSGT(left, right, "int_gt");
        if (b->left->type == Type::FLOAT)
            return llvm_builder.CreateFCmpUGT(left, right, "float_gt");
    }
    else if (b->loc.type == PLUS)
    {
        if (b->type == Type::INT)
            return llvm_builder.CreateAdd(left, right, "int_add");
        if (b->type == Type::FLOAT)
            return llvm_builder.CreateFAdd(left, right, "float_add");
    }
    else if (b->loc.type == MINUS)
    {
        if (b->type == Type::INT)
            return llvm_builder.CreateSub(left, right, "int_sub");
        if (b->type == Type::FLOAT)
            return llvm_builder.CreateFSub(left, right, "float_sub");
    }
    else if (b->loc.type == ASTERIX)
    {
        if (b->type == Type::INT)
            return llvm_builder.CreateMul(left, right, "int_mul");
        if (b->type == Type::FLOAT)
            return llvm_builder.CreateFMul(left, right, "float_mul");
    }
    else if (b->loc.type == DIV)
    {
        if (b->type == Type::INT)
            return llvm_builder.CreateSDiv(left, right, "int_div");
        if (b->type == Type::FLOAT)
            return llvm_builder.CreateFDiv(left, right, "float_div");
    }
    else if (b->loc.type == MOD)
    {
        // https: //llvm.org/doxygen/IntegerDivision_8cpp_source.html
        return nullptr;
    }
    return nullptr;
}

llvm::Value *Compiler::CompileIdentifierNode(IdentifierNode *i)
{
    return llvm_builder.CreateLoad(TypeToLLVMType(i->type), named_values[i->GetName()], i->GetName());
}

llvm::Value *Compiler::CompileAssignNode(AssignNode *a)
{
    llvm::Value *rhs = a->val->Compile(*this);
    llvm_builder.CreateStore(rhs, named_values[a->name]);
    return rhs;
}

llvm::Value *Compiler::CompileFunctionCallNode(FunctionCallNode *fc)
{
    llvm::Function *func = llvm_module->getFunction(fc->loc.lexeme);

    std::vector<llvm::Value *> llvm_args;
    for (auto &arg : fc->args)
        llvm_args.push_back(arg->Compile(*this));

    return llvm_builder.CreateCall(func, llvm_args, "func_call");
}

llvm::Value *Compiler::CompileCoercionNode(CoercionNode *c)
{
    llvm::Value *exp = c->exp->Compile(*this);
    if (c->type == Type::INT)
    {
        if (c->exp->type == Type::BOOL)
            return llvm_builder.CreateIntCast(exp, llvm::Type::getInt32Ty(llvm_context), true, "bool_to_int");
        else if (c->exp->type == Type::FLOAT)
            return llvm_builder.CreateFPToSI(exp, llvm::Type::getInt32Ty(llvm_context), "float_to_int");
    }
    else if (c->type == Type::BOOL)
    {
        if (c->exp->type == Type::INT)
            return llvm_builder.CreateIntCast(exp, llvm::Type::getInt1Ty(llvm_context), false, "int_to_bool");
    }
    else if (c->type == Type::FLOAT)
    {
        if (c->exp->type == Type::INT)
            return llvm_builder.CreateSIToFP(exp, llvm::Type::getFloatTy(llvm_context), "int_to_float");
    }
    // should never be reached
    return nullptr;
}

//------------------------------------------STATEMENTS------------------------------------------//

llvm::AllocaInst *Compiler::CreateAlloca(const Type var_type, const std::string &name)
{
    if (var_type == Type::INT)
        return llvm_builder.CreateAlloca(llvm::Type::getInt32Ty(llvm_context), nullptr, name);
    else if (var_type == Type::BOOL)
        return llvm_builder.CreateAlloca(llvm::Type::getInt1Ty(llvm_context), nullptr, name);
    else if (var_type == Type::FLOAT)
        return llvm_builder.CreateAlloca(llvm::Type::getFloatTy(llvm_context), nullptr, name);

    return nullptr;
}

llvm::Type *Compiler::TypeToLLVMType(const Type type)
{
    if (type == Type::INT)
        return llvm::Type::getInt32Ty(llvm_context);
    else if (type == Type::BOOL)
        return llvm::Type::getInt1Ty(llvm_context);
    else
        return llvm::Type::getFloatTy(llvm_context);
}

void Compiler::CompileExprStmtNode(ExprStmtNode *es)
{
    es->exp->Compile(*this);
}

void Compiler::CompileWhileNode(WhileNode *)
{
}

void Compiler::CompileIfElseNode(IfElseNode *ie)
{
    llvm::Value *condition = ie->condition->Compile(*this);

    llvm::Function *cur_func = llvm_builder.GetInsertBlock()->getParent();

    llvm::BasicBlock *then_bb = llvm::BasicBlock::Create(llvm_context, "then", cur_func);
    llvm::BasicBlock *else_bb = llvm::BasicBlock::Create(llvm_context, "else");
    llvm::BasicBlock *after_bb = llvm::BasicBlock::Create(llvm_context, "after");

    llvm_builder.CreateCondBr(condition, then_bb, else_bb);
    llvm_builder.SetInsertPoint(then_bb);

    // ie->then->Compile(*this);
    SP<BlockNode> then = std::dynamic_pointer_cast<BlockNode>(ie->then);
    for (auto &decl : then->local_decls)
        decl->Compile(*this);
    for (auto &stmt : then->stmts)
        stmt->Compile(*this);

    llvm_builder.CreateBr(after_bb);

    then_bb = llvm_builder.GetInsertBlock();
    cur_func->getBasicBlockList().push_back(else_bb);
    llvm_builder.SetInsertPoint(else_bb);

    // ie->other->Compile(*this);
    SP<BlockNode> other = std::dynamic_pointer_cast<BlockNode>(ie->other);
    if (other != nullptr)
    {
        for (auto &decl : other->local_decls)
            decl->Compile(*this);
        for (auto &stmt : other->stmts)
            stmt->Compile(*this);
    }

    llvm_builder.CreateBr(after_bb);
    else_bb = llvm_builder.GetInsertBlock();

    cur_func->getBasicBlockList().push_back(after_bb);
    llvm_builder.SetInsertPoint(after_bb);
}

void Compiler::CompileReturnNode(ReturnNode *r)
{
    llvm::Value *ret_val = r->ret->Compile(*this);
    llvm_builder.CreateRet(ret_val);
}

void Compiler::CompileLocalDeclNode(LocalDeclNode *ld)
{
    llvm::AllocaInst *var_alloc = CreateAlloca(ld->type, ld->name);
    named_values[ld->name] = var_alloc;

    if (ld->type == Type::INT)
        llvm_builder.CreateStore(S_INT32_ZERO, var_alloc);
    else if (ld->type == Type::FLOAT)
        llvm_builder.CreateStore(F32_ZERO, var_alloc);
    else if (ld->type == Type::BOOL)
        llvm_builder.CreateStore(BOOL_FALSE, var_alloc);
}

void Compiler::CompileBlockNode(BlockNode *b)
{
    llvm::BasicBlock *block_body = llvm::BasicBlock::Create(llvm_context, "block_body", cur_func);
    llvm::BasicBlock *after = llvm::BasicBlock::Create(llvm_context, "after_block");

    llvm_builder.CreateBr(block_body);
    llvm_builder.SetInsertPoint(block_body);

    for (auto &decl : b->local_decls)
        decl->Compile(*this);
    for (auto &stmt : b->stmts)
        stmt->Compile(*this);

    llvm_builder.CreateBr(after);
    cur_func->getBasicBlockList().push_back(after);
    llvm_builder.SetInsertPoint(after);
}

void Compiler::CompileFunctionDeclNode(FunctionDeclNode *fd)
{
    std::vector<llvm::Type *> llvm_arg_types;
    for (const auto &arg : fd->args)
        llvm_arg_types.push_back(TypeToLLVMType(arg.first));

    llvm::FunctionType *llvm_decl = llvm::FunctionType::get(TypeToLLVMType(fd->ret), llvm_arg_types, false);
    llvm::Function *func = llvm::Function::Create(llvm_decl, llvm::Function::ExternalLinkage, fd->name, llvm_module.get());

    llvm::BasicBlock *func_body = llvm::BasicBlock::Create(llvm_context, "function_body", func);
    llvm_builder.SetInsertPoint(func_body);

    unsigned idx = 0;
    for (auto &arg : func->args())
        arg.setName(fd->args[idx++].second);

    std::unordered_map<std::string, Type> name_type;
    for (const auto &arg : fd->args)
        name_type[arg.second] = arg.first;

    named_values.clear();
    for (auto &arg : func->args())
    {
        llvm::AllocaInst *alloca = CreateAlloca(name_type[std::string(arg.getName())], std::string(arg.getName()));
        llvm_builder.CreateStore(&arg, alloca);
        named_values[std::string(arg.getName())] = alloca;
    }

    cur_func = func;

    SP<BlockNode> body_as_block = std::dynamic_pointer_cast<BlockNode>(fd->body);
    for (auto &decl : body_as_block->local_decls)
        decl->Compile(*this);
    for (auto &stmt : body_as_block->stmts)
        stmt->Compile(*this);

    if (fd->ret == Type::INT)
        llvm_builder.CreateRet(S_INT32_ZERO);
    else if (fd->ret == Type::BOOL)
        llvm_builder.CreateRet(BOOL_FALSE);
    else if (fd->ret == Type::FLOAT)
        llvm_builder.CreateRet(F32_ZERO);
    else
        llvm_builder.CreateRet(nullptr);

    cur_func = nullptr;
}

void Compiler::CompileExternNode(ExternNode *e)
{
    std::vector<llvm::Type *> llvm_arg_types;
    for (const auto &arg : e->args)
        llvm_arg_types.push_back(TypeToLLVMType(arg.first));

    llvm::FunctionType *llvm_decl = llvm::FunctionType::get(TypeToLLVMType(e->ret), llvm_arg_types, false);
    llvm::Function *func = llvm::Function::Create(llvm_decl, llvm::Function::ExternalLinkage, e->name, llvm_module.get());

    unsigned idx = 0;
    for (auto &arg : func->args())
        arg.setName(e->args[idx++].second);
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

void ExprStmtNode::Compile(Compiler &c)
{
    c.CompileExprStmtNode(this);
}

void WhileNode::Compile(Compiler &c)
{
    c.CompileWhileNode(this);
}

void IfElseNode::Compile(Compiler &c)
{
    c.CompileIfElseNode(this);
}

void ReturnNode::Compile(Compiler &c)
{
    c.CompileReturnNode(this);
}

void LocalDeclNode::Compile(Compiler &c)
{
    c.CompileLocalDeclNode(this);
}

void BlockNode::Compile(Compiler &c)
{
    c.CompileBlockNode(this);
}

void FunctionDeclNode::Compile(Compiler &c)
{
    c.CompileFunctionDeclNode(this);
}

void ExternNode::Compile(Compiler &c)
{
    c.CompileExternNode(this);
}

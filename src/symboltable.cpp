#include "symboltable.h"

std::ostream &operator<<(std::ostream &out, const VarID &v)
{
    out << ExprType::to_string(v.type) << " " << v.name;
    return out;
}

bool operator==(const VarID &lhs, const VarID &rhs)
{
    return lhs.type == rhs.type && lhs.name == rhs.name;
}

std::optional<Type> SymbolTable::CheckUnaryOperatorUse(const TOKEN_TYPE op, const Type right) const
{
    const OperatorUse candidate(Type::VOID, op, right);
    if (OperatorMap.find(candidate) != OperatorMap.end())
        return OperatorMap.at(candidate);
    return std::nullopt;
}

std::optional<Type> SymbolTable::CheckBinaryOperatorUse(const Type left, const TOKEN_TYPE op, const Type right) const
{
    const OperatorUse candidate(left, op, right);
    if (OperatorMap.find(candidate) != OperatorMap.end())
        return OperatorMap.at(candidate);
    return std::nullopt;
}

bool SymbolTable::CanAssign(const Type target, const Type value) const
{
    if (target == Type::INT)
        return (value == Type::INT) || (value == Type::FLOAT);
    else if (target == Type::FLOAT)
        return (value == Type::INT) || (value == Type::FLOAT);
    else if (target == Type::BOOL)
        return (value == Type::INT) || (value == Type::BOOL);
    return false;
}

bool SymbolTable::AddVariable(const Type type, const std::string &name)
{
    VarID candidate(type, name, depth);

    for (int i = var_table[name].size() - 1; i >= 0; i--)
    {
        if (candidate == var_table[name][i])
            return false;
    }

    var_table[name].push_back(candidate);
    return true;
}

std::optional<VarID> SymbolTable::ResolveVariable(const std::string &name)
{
    std::vector<VarID> same_names = var_table[name];

    if (same_names.size() == 0)
        return std::nullopt;
    else
        return same_names.back();
}

void SymbolTable::ClearCurrentScope()
{
    for (auto &kv : var_table)
    {
        if (kv.second.size() > 0 && kv.second.back().depth == depth)
        {
            while (kv.second.size() > 0)
                kv.second.pop_back();

            if (kv.second.size() == 1 && kv.second[0].depth == depth)
                kv.second.clear();
        }
    }
}

bool SymbolTable::AddFunction(const Type ret, const std::string &name, const std::vector<Type> &args)
{
    if (ResolveFunction(name, args))
        return false;
    func_table[name].push_back(FuncID(ret, name, args));
    return true;
}

std::optional<FuncID> SymbolTable::ResolveFunction(const std::string &name, const std::vector<Type> &args)
{
    for (const auto &func : func_table[name])
    {
        if (AreTypesEqual(func.args, args))
            return func;
    }

    for (const auto &func : func_table[name])
    {
        if (CanAssignAll(func.args, args))
            return func;
    }

    return std::nullopt;
}

bool SymbolTable::AreTypesEqual(const std::vector<Type> &actual, const std::vector<Type> &supplied) const
{
    if (actual.size() != supplied.size())
        return false;

    for (size_t i = 0; i < actual.size(); i++)
    {
        if (actual[i] != supplied[i])
            return false;
    }

    return true;
}

bool SymbolTable::CanAssignAll(const std::vector<Type> &actual, const std::vector<Type> &supplied) const
{
    if (actual.size() != supplied.size())
        return false;

    for (size_t i = 0; i < actual.size(); i++)
    {
        if (!CanAssign(actual[i], supplied[i]))
            return false;
    }

    return true;
}
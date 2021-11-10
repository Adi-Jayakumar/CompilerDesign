#pragma once
#include "token.h"
#include "type.h"
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

struct VarID
{
    Type type;
    std::string name;
    size_t depth;

    VarID(const Type _type, const std::string &_name, const size_t _depth) : type(_type), name(_name), depth(_depth){};
};

std::ostream &operator<<(std::ostream &out, const VarID &v);
bool operator==(const VarID &lhs, const VarID &rhs);

struct FuncID
{
    Type ret;
    std::string name;
    std::vector<Type> args;

    FuncID(const Type _ret,
           const std::string &_name,
           const std::vector<Type> &_args) : ret(_ret),
                                             name(_name),
                                             args(_args){};
};

class SymbolTable
{
    std::unordered_map<std::string, std::vector<VarID>> var_table;
    std::unordered_map<std::string, std::vector<FuncID>> func_table;

    struct OperatorUse
    {
        Type left;
        TOKEN_TYPE op;
        Type right;

        OperatorUse(const Type _left, const TOKEN_TYPE _op, const Type _right) : left(_left), op(_op), right(_right){};

        bool operator==(const OperatorUse &other) const
        {
            return (left == other.left) && (op == other.op) && (right == other.right);
        }
    };

    struct OperatorUseHash
    {
        size_t operator()(const OperatorUse &o) const
        {
            return std::hash<Type>()(o.left) ^ std::hash<TOKEN_TYPE>()(o.op) ^ std::hash<Type>()(o.right);
        }
    };

    const std::unordered_map<OperatorUse, Type, OperatorUseHash> OperatorMap{
        // ||
        {{Type::BOOL, OR, Type::BOOL}, Type::BOOL},
        {{Type::INT, OR, Type::INT}, Type::BOOL},
        {{Type::BOOL, OR, Type::INT}, Type::BOOL},
        {{Type::INT, OR, Type::BOOL}, Type::BOOL},
        // &&
        {{Type::BOOL, AND, Type::BOOL}, Type::BOOL},
        {{Type::INT, AND, Type::INT}, Type::BOOL},
        {{Type::BOOL, AND, Type::INT}, Type::BOOL},
        {{Type::INT, AND, Type::BOOL}, Type::BOOL},
        // ==
        {{Type::INT, EQ, Type::INT}, Type::BOOL},
        {{Type::FLOAT, EQ, Type::FLOAT}, Type::BOOL},
        {{Type::FLOAT, EQ, Type::INT}, Type::BOOL},
        {{Type::INT, EQ, Type::FLOAT}, Type::BOOL},
        {{Type::BOOL, EQ, Type::BOOL}, Type::BOOL},
        // !=
        {{Type::INT, NE, Type::INT}, Type::BOOL},
        {{Type::FLOAT, NE, Type::FLOAT}, Type::BOOL},
        {{Type::FLOAT, NE, Type::INT}, Type::BOOL},
        {{Type::INT, NE, Type::FLOAT}, Type::BOOL},
        {{Type::BOOL, NE, Type::BOOL}, Type::BOOL},
        // <=
        {{Type::INT, LE, Type::INT}, Type::BOOL},
        {{Type::FLOAT, LE, Type::FLOAT}, Type::BOOL},
        {{Type::FLOAT, LE, Type::INT}, Type::BOOL},
        {{Type::INT, LE, Type::FLOAT}, Type::BOOL},
        // <
        {{Type::INT, LT, Type::INT}, Type::BOOL},
        {{Type::FLOAT, LT, Type::FLOAT}, Type::BOOL},
        {{Type::FLOAT, LT, Type::INT}, Type::BOOL},
        {{Type::INT, LT, Type::FLOAT}, Type::BOOL},
        // >=
        {{Type::INT, GE, Type::INT}, Type::BOOL},
        {{Type::FLOAT, GE, Type::FLOAT}, Type::BOOL},
        {{Type::FLOAT, GE, Type::INT}, Type::BOOL},
        {{Type::INT, GE, Type::FLOAT}, Type::BOOL},
        // >
        {{Type::INT, GT, Type::INT}, Type::BOOL},
        {{Type::FLOAT, GT, Type::FLOAT}, Type::BOOL},
        {{Type::FLOAT, GT, Type::INT}, Type::BOOL},
        {{Type::INT, GT, Type::FLOAT}, Type::BOOL},
        // +
        {{Type::INT, PLUS, Type::INT}, Type::INT},
        {{Type::FLOAT, PLUS, Type::FLOAT}, Type::FLOAT},
        {{Type::FLOAT, PLUS, Type::INT}, Type::FLOAT},
        {{Type::INT, PLUS, Type::FLOAT}, Type::FLOAT},
        // -
        {{Type::INT, MINUS, Type::INT}, Type::INT},
        {{Type::FLOAT, MINUS, Type::FLOAT}, Type::FLOAT},
        {{Type::FLOAT, MINUS, Type::INT}, Type::FLOAT},
        {{Type::INT, MINUS, Type::FLOAT}, Type::FLOAT},
        // - (unary)
        {{Type::VOID, MINUS, Type::INT}, Type::INT},
        {{Type::VOID, MINUS, Type::FLOAT}, Type::FLOAT},
        // *
        {{Type::INT, ASTERIX, Type::INT}, Type::INT},
        {{Type::FLOAT, ASTERIX, Type::FLOAT}, Type::FLOAT},
        {{Type::FLOAT, ASTERIX, Type::INT}, Type::FLOAT},
        {{Type::INT, ASTERIX, Type::FLOAT}, Type::FLOAT},
        // /
        {{Type::INT, DIV, Type::INT}, Type::INT},
        {{Type::FLOAT, DIV, Type::FLOAT}, Type::FLOAT},
        {{Type::FLOAT, DIV, Type::INT}, Type::FLOAT},
        {{Type::INT, DIV, Type::FLOAT}, Type::FLOAT},
        // %
        {{Type::INT, MOD, Type::INT}, Type::INT},
        // ! (unary)
        {{Type::VOID, NOT, Type::INT}, Type::BOOL},
        {{Type::VOID, NOT, Type::BOOL}, Type::BOOL},
    };

    bool AreTypesEqual(const std::vector<Type> &actual, const std::vector<Type> &supplied) const;
    bool CanAssignAll(const std::vector<Type> &actual, const std::vector<Type> &supplied) const;

    bool IsDefinedBefore(const std::string &name, const std::vector<Type> &args);

public:
    size_t depth;
    SymbolTable() = default;

    std::optional<Type> CheckUnaryOperatorUse(const TOKEN_TYPE op, const Type right) const;
    std::optional<Type> CheckBinaryOperatorUse(const Type left, const TOKEN_TYPE op, const Type right) const;

    bool CanAssign(const Type target, const Type val) const;

    //-------------------------VARIABLES-------------------------//

    bool AddVariable(const Type type, const std::string &name);
    // returns std::optional of last variable in scope or nullptr if no such variable exists
    std::optional<VarID> ResolveVariable(const std::string &name);

    void ClearCurrentScope();

    //-------------------------FUNCTIONS-------------------------//

    bool AddFunction(const Type ret, const std::string &name, const std::vector<Type> &args);
    // returns a std::optional of the corresponding function or nullptr if no such function exists
    std::optional<FuncID> ResolveFunction(const std::string &name, const std::vector<Type> &args);
};
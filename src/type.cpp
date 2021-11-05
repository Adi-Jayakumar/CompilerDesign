#include "type.h"

std::string ExprType::to_string(const Type type)
{
    switch (type)
    {
    case Type::VOID:
    {
        return "void";
    }
    case Type::INT:
    {
        return "int";
    }
    case Type::BOOL:
    {
        return "bool";
    }
    case Type::FLOAT:
    {
        return "float";
    }
    }
    // should never be reached
    return "";
}

std::ostream &operator<<(std::ostream &out, const Type type)
{
    out << ExprType::to_string(type);
    return out;
}

#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

enum class Type
{
    VOID,
    INT,
    BOOL,
    FLOAT,
};

namespace ExprType
{
    std::string to_string(const Type type);
}

std::ostream &operator<<(std::ostream &out, const Type type);

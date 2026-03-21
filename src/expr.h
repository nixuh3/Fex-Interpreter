#pragma once

#include "token.h"
#include <variant>

namespace fex {

struct Expr;

struct Assign {
    const Token name;
    const Expr* value;
};

struct Binary {
    const Expr* left;
    const Token op;
    const Expr* right;
};

struct Unary {
    const Token op;
    const Expr* right;
};

struct Grouping {
    const Expr* expression;
};

struct Literal {
    const Value value;
};

struct Variable {
    const Token name;
};

struct Expr : std::variant<Assign, Binary, Grouping, Literal, Unary, Variable> {
    using variant::variant; // inherit constructors
};

} // namespace fex

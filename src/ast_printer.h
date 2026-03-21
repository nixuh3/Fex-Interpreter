#pragma once

#include "expr.h"
#include <string>

namespace fex {

class AstPrinter {
  public:
    std::string Print(const Expr* expr) {
        return std::visit([this](auto&& node) { return Visit(node); }, *expr);
    }

  private:
    std::string Visit(const Assign& expr) { return ""; }

    std::string Visit(const Binary& expr) {
        return Parenthesize(expr.op.lexeme, expr.left, expr.right);
    }

    std::string Visit(const Unary& expr) { return Parenthesize(expr.op.lexeme, expr.right); }

    std::string Visit(const Grouping& expr) { return Parenthesize("group", expr.expression); }

    std::string Visit(const Literal& expr) {
        if (expr.value == Value{ std::monostate{} }) {
            return "null";
        }
        return ValToStr(expr.value);
    }

    std::string Visit(const Variable& expr) { return expr.name.lexeme; }

    template <typename... Args>
    std::string Parenthesize(const std::string& name, Args... exprs) {
        std::string str = "(" + name;

        ((str += " " + Print(std::forward<Args>(exprs))), ...);

        str += ")";
        return str;
    }
};

} // namespace fex

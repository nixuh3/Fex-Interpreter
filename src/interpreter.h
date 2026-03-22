#pragma once

#include "token.h"

namespace fex {

struct Expr;
struct Assign;
struct Binary;
struct Unary;
struct Grouping;
struct Literal;
struct Variable;

class Interpreter {
  public:
    void Interpret(const Expr* expr);

  private:
    Value Visit(const Assign& expr);
    Value Visit(const Binary& expr);
    Value Visit(const Unary& expr);
    Value Visit(const Grouping& expr);
    Value Visit(const Literal& expr);
    Value Visit(const Variable& expr);
    Value Evaluate(const Expr* expr);

    void CheckNumberOperand(const Token& op, const Value& operand);
    void CheckNumberOperands(const Token& op, const Value& left, const Value& right);
    bool IsTruthy(const Value& value);
    bool IsEqual(const Value& a, const Value& b);
};

} // namespace fex

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
    static void Interpret(const Expr* expr);

  private:
    static Value Visit(const Assign& expr);
    static Value Visit(const Binary& expr);
    static Value Visit(const Unary& expr);
    static Value Visit(const Grouping& expr);
    static Value Visit(const Literal& expr);
    static Value Visit(const Variable& expr);
    static Value Evaluate(const Expr* expr);

    static void CheckNumberOperand(const Token& op, const Value& operand);
    static void CheckNumberOperands(const Token& op, const Value& left, const Value& right);
    static bool IsTruthy(const Value& value);
    static bool IsEqual(const Value& a, const Value& b);
    static double GetAsNumber(const Value& value);
};

} // namespace fex

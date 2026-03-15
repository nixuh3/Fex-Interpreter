#pragma once
#include "token.h"
namespace fex {
class Assign;
class Binary;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;

class Visitor {
  public:
    virtual ~Visitor() {}
    virtual void VisitAssignExpr(const Assign* expr) = 0;
    virtual void VisitBinaryExpr(const Binary* expr) = 0;
    virtual void VisitGroupingExpr(const Grouping* expr) = 0;
    virtual void VisitLiteralExpr(const Literal* expr) = 0;
    virtual void VisitLogicalExpr(const Logical* expr) = 0;
    virtual void VisitUnaryExpr(const Unary* expr) = 0;
    virtual void VisitVariableExpr(const Variable* expr) = 0;
};
class Expr {
  public:
    virtual ~Expr() {}
    virtual void Accept(Visitor& visitor) const = 0;
};
class Assign : public Expr {
  public:
    Assign(Token name, const Expr* value)
        : m_name(name), m_value(value) {}
    virtual void Accept(Visitor& visitor) const override { visitor.VisitAssignExpr(this); }
  private:
    Token m_name;
    const Expr* m_value;
};
class Binary : public Expr {
  public:
    Binary(const Expr* left, Token op, const Expr* right)
        : m_left(left), m_op(op), m_right(right) {}
    virtual void Accept(Visitor& visitor) const override { visitor.VisitBinaryExpr(this); }
  private:
    const Expr* m_left;
    Token m_op;
    const Expr* m_right;
};
class Grouping : public Expr {
  public:
    Grouping(const Expr* expression)
        : m_expression(expression) {}
    virtual void Accept(Visitor& visitor) const override { visitor.VisitGroupingExpr(this); }
  private:
    const Expr* m_expression;
};
class Literal : public Expr {
  public:
    Literal(Value value)
        : m_value(value) {}
    virtual void Accept(Visitor& visitor) const override { visitor.VisitLiteralExpr(this); }
  private:
    Value m_value;
};
class Logical : public Expr {
  public:
    Logical(const Expr* left, Token op, const Expr* right)
        : m_left(left), m_op(op), m_right(right) {}
    virtual void Accept(Visitor& visitor) const override { visitor.VisitLogicalExpr(this); }
  private:
    const Expr* m_left;
    Token m_op;
    const Expr* m_right;
};
class Unary : public Expr {
  public:
    Unary(Token op, const Expr* right)
        : m_op(op), m_right(right) {}
    virtual void Accept(Visitor& visitor) const override { visitor.VisitUnaryExpr(this); }
  private:
    Token m_op;
    const Expr* m_right;
};
class Variable : public Expr {
  public:
    Variable(Token name)
        : m_name(name) {}
    virtual void Accept(Visitor& visitor) const override { visitor.VisitVariableExpr(this); }
  private:
    Token m_name;
};
} // namespace fex

#include "interpreter.h"
#include "expr.h"
#include "fex.h"
#include "runtime_error.h"
#include "utils.h"
#include <iostream>

namespace fex {

void Interpreter::Interpret(const Expr* expr) {
    try {
        Value value = Evaluate(expr);
        std::cout << ValToStr(value) << "\n";
    } catch (RuntimeError e) {
        FexInterpreter::RuntimeError_(e);
    }
}

Value Interpreter::Visit(const Assign& expr) {
    return {};
}

Value Interpreter::Visit(const Binary& expr) {
    Value left = Evaluate(expr.left);
    Value right = Evaluate(expr.right);

    switch (expr.op.type) {
        case GREATER:
            CheckNumberOperands(expr.op, left, right);
            return double(std::get<double>(left) > std::get<double>(right));
        case GREATER_EQUAL:
            CheckNumberOperands(expr.op, left, right);
            return double(std::get<double>(left) >= std::get<double>(right));
        case LESS:
            CheckNumberOperands(expr.op, left, right);
            return double(std::get<double>(left) < std::get<double>(right));
        case LESS_EQUAL:
            CheckNumberOperands(expr.op, left, right);
            return double(std::get<double>(left) <= std::get<double>(right));
        case MINUS:
            CheckNumberOperands(expr.op, left, right);
            return std::get<double>(left) - std::get<double>(right);
        case SLASH:
            CheckNumberOperands(expr.op, left, right);
            return std::get<double>(left) / std::get<double>(right);
        case STAR:
            CheckNumberOperands(expr.op, left, right);
            return std::get<double>(left) * std::get<double>(right);
        case PLUS:
            return std::visit(
                [&expr](auto&& l, auto&& r) -> Value {
                    using L = std::decay_t<decltype(l)>;
                    using R = std::decay_t<decltype(r)>;

                    if constexpr (std::is_same_v<L, double> && std::is_same_v<R, double>) {
                        return l + r;
                    } else if constexpr (std::is_same_v<L, std::string> &&
                                         std::is_same_v<R, std::string>) {
                        return l + r;
                    } else if constexpr (std::is_same_v<L, double> &&
                                         std::is_same_v<R, std::string>) {
                        return FormatDouble(l) + r;
                    } else if constexpr (std::is_same_v<L, std::string> &&
                                         std::is_same_v<R, double>) {
                        return l + FormatDouble(r);
                    } else {
                        throw RuntimeError{ expr.op,
                                            "Operands must be two numbers or two strings." };
                    }
                },
                left, right);
        case EXCLAM_EQUAL: return double(!IsEqual(left, right));
        case EQUAL_EQUAL: return double(IsEqual(left, right));
        case AMP_AMP: return double(IsTruthy(left) && IsTruthy(right));
        case PIPE_PIPE: return double(IsTruthy(left) || IsTruthy(right));
    }

    // Unreachable.
    return {};
}

Value Interpreter::Visit(const Unary& expr) {
    Value right = Evaluate(expr.right);

    switch (expr.op.type) {
        case EXCLAM: !IsTruthy(right);
        case MINUS: CheckNumberOperand(expr.op, right); return -std::get<double>(right);
        case PLUS: CheckNumberOperand(expr.op, right); return right;
    }

    // Unreachable.
    return {};
}

Value Interpreter::Visit(const Grouping& expr) {
    return Evaluate(expr.expression);
}

Value Interpreter::Visit(const Literal& expr) {
    return expr.value;
}

Value Interpreter::Visit(const Variable& expr) {
    return {};
}

Value Interpreter::Evaluate(const Expr* expr) {
    return std::visit([this](auto&& node) { return Visit(node); }, *expr);
}

void Interpreter::CheckNumberOperand(const Token& op, const Value& operand) {
    if (std::holds_alternative<double>(operand)) {
        return;
    }
    throw RuntimeError{ op, "Operand must be a number." };
}

void Interpreter::CheckNumberOperands(const Token& op, const Value& left, const Value& right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        return;
    }
    throw RuntimeError{ op, "Operands must be numbers." };
}

bool Interpreter::IsTruthy(const Value& value) {
    return std::visit(
        [](auto&& v) -> bool {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, double>) {
                return (bool)v;
            } else if constexpr (std::is_same_v<T, std::monostate>) {
                return false;
            } else {
                return true;
            }
        },
        value);
}

bool Interpreter::IsEqual(const Value& a, const Value& b) {
    return std::visit(
        [](auto&& l, auto&& r) -> bool {
            using L = std::decay_t<decltype(l)>;
            using R = std::decay_t<decltype(r)>;

            if constexpr (std::is_same_v<L, std::monostate> && std::is_same_v<R, std::monostate>) {
                return true; // both are monostate
            } else if constexpr (std::is_same_v<L, std::monostate> ||
                                 std::is_same_v<R, std::monostate>) {
                return false; // one is monostate, the other is not
            } else if constexpr (std::is_same_v<L, R>) {
                return l == r; // same type, compare normally
            } else {
                return false; // different types, not equal
            }
        },
        a, b);
}

} // namespace fex
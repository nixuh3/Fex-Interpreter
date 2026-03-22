#include "parser.h"
#include "arena.h"
#include "fex.h"

namespace fex {

Parser::Parser(const std::vector<Token>& tokens, Arena& arena)
    : m_tokens(tokens), m_current(0), m_arena(arena) {}

const Expr* Parser::Parse() {
    try {
        return Expression();
    } catch (ParseError error) {
        return nullptr;
    }
}

const Expr* Parser::Expression() {
    return LogicalOr();
}

const Expr* Parser::LogicalOr() {
    const Expr* expr = LogicalAnd();

    while (Match(PIPE_PIPE)) {
        Token op = Previous();
        const Expr* right = LogicalAnd();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::LogicalAnd() {
    const Expr* expr = Equality();

    while (Match(AMP_AMP)) {
        Token op = Previous();
        const Expr* right = Equality();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::Equality() {
    const Expr* expr = Relational();

    while (Match(EXCLAM_EQUAL, EQUAL_EQUAL)) {
        Token op = Previous();
        const Expr* right = Relational();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::Relational() {
    const Expr* expr = Term();

    while (Match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token op = Previous();
        const Expr* right = Term();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::Term() {
    const Expr* expr = Factor();

    while (Match(MINUS, PLUS)) {
        Token op = Previous();
        const Expr* right = Factor();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::Factor() {
    const Expr* expr = Unary_();

    while (Match(STAR, SLASH, PERCENT)) {
        Token op = Previous();
        const Expr* right = Unary_();
        expr = m_arena.Alloc<Expr>(Binary{ expr, op, right });
    }

    return expr;
}

const Expr* Parser::Unary_() {
    if (Match(EXCLAM, PLUS, MINUS)) {
        Token op = Previous();
        const Expr* right = Unary_();
        return m_arena.Alloc<Expr>(Unary{ op, right });
    }

    return Primary_();
}

const Expr* Parser::Primary_() {
    if (Match(FALSE)) {
        return m_arena.Alloc<Expr>(Literal{ 0.0 });
    }
    if (Match(TRUE)) {
        return m_arena.Alloc<Expr>(Literal{ 1.0 });
    }
    if (Match(NUL)) {
        return m_arena.Alloc<Expr>(Literal{ std::monostate{} });
    }

    if (Match(NUMBER, STRING)) {
        return m_arena.Alloc<Expr>(Literal{ Previous().literal });
    }

    if (Match(LEFT_PAREN)) {
        const Expr* expr = Expression();
        Consume(RIGHT_PAREN, "Expect ')' after expression.");
        return m_arena.Alloc<Expr>(Grouping{ expr });
    }

    throw Error(Peek(), "Expect expression.");
}

template <typename... Args>
bool Parser::Match(Args... args) {
    if (((Check(args) ? (Advance(), true) : false) || ...)) {
        return true;
    }
    return false;
}

bool Parser::Check(TokenType type) {
    if (IsAtEnd()) {
        return false;
    }
    return Peek().type == type;
}

Token Parser::Advance() {
    if (!IsAtEnd()) {
        m_current++;
    }
    return Previous();
}

bool Parser::IsAtEnd() {
    return Peek().type == END;
}

Token Parser::Peek() {
    return m_tokens[m_current];
}

Token Parser::Previous() {
    return m_tokens[m_current - 1];
}

Token Parser::Consume(TokenType type, std::string_view message) {
    if (Check(type)) {
        return Advance();
    }
    throw Error(Peek(), message);
}

Parser::ParseError Parser::Error(Token token, std::string_view message) {
    FexInterpreter::Error(token, message);
    return ParseError();
}

} // namespace fex

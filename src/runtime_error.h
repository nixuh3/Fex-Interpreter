#pragma once

#include "token.h"
#include <stdexcept>

namespace fex {

struct RuntimeError : public std::runtime_error {
    RuntimeError(const Token& token, const std::string& msg)
        : std::runtime_error(msg), token(token) {}

    Token token;
};

} // namespace fex

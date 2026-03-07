#pragma once

#include <string>

namespace fex {

class Token;

void Error(int line, std::string_view message);
void Error(const Token& token, std::string_view message);

} // namespace fex

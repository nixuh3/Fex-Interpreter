#pragma once

#include <string>

namespace fex {

class Token;

void FexError(int line, std::string_view message);
void FexError(const Token& token, std::string_view message);

} // namespace fex

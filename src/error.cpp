#include "error.h"
#include "fex.h"
#include "token.h"

namespace fex {

void FexError(int line, std::string_view message) {
    FexInterpreter::Report(line, "", message);
}

void FexError(const Token& token, std::string_view message) {
    if (token.type == END) {
        FexInterpreter::Report(token.line, " at end", message);
    } else {
        FexInterpreter::Report(token.line, " at '" + token.lexeme + "'", message);
    }
}

} // namespace fex

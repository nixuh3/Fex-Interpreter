#include "error.h"
#include "fex.h"
#include "token.h"

namespace fex {

void FexError(int line, std::string_view message) {
    FexInterpreter::Report(line, "", message);
}

void FexError(const Token& token, std::string_view message) {
    if (token.GetType() == END) {
        FexInterpreter::Report(token.GetLine(), " at end", message);
    } else {
        FexInterpreter::Report(token.GetLine(), " at '" + token.GetLexeme() + "'", message);
    }
}

} // namespace fex

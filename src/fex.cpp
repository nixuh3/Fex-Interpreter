#include "fex.h"
#include "ast_printer.h"
#include "parser.h"
#include "runtime_error.h"
#include "scanner.h"
#include <fstream>
#include <iostream>

namespace fex {

void FexInterpreter::RunFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cout << "Failed to open file: " << path << "\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string buffer;
    buffer.resize(size);

    file.read(buffer.data(), size);
    file.close();

    Run(buffer);

    if (s_hadError || s_hadRuntimeError) {
        std::exit(1);
    }
}

void FexInterpreter::RunREPL() {
    std::string line;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        Run(line);
        s_hadError = false;
    }
}

void FexInterpreter::Error(int line, std::string_view message) {
    Report(line, "", message);
}

void FexInterpreter::Error(const Token& token, std::string_view message) {
    if (token.type == END) {
        Report(token.line, " at end", message);
    } else {
        Report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void FexInterpreter::RuntimeError_(const RuntimeError& error) {
    std::cout << std::string(error.what()) + " [line " + std::to_string(error.token.line) + "]\n";
    s_hadRuntimeError = true;
}

void FexInterpreter::Run(std::string_view source) {
    Scanner scanner(source);
    const auto& tokens = scanner.ScanTokens();

    for (const auto& token : tokens) {
        std::cout << token.ToStr() << "\n";
    }

    Parser parser(tokens, s_arena);
    auto expr = parser.Parse();

    if (s_hadError) {
        return;
    }

    AstPrinter printer;
    std::cout << "AST: " << printer.Print(expr) << "\n";

    s_interpreter.Interpret(expr);
}

void FexInterpreter::Report(int line, std::string_view where, std::string_view msg) {
    std::cout << "[line " << line << "] Error" << where << ": " << msg << "\n";
    s_hadError = true;
}

} // namespace fex

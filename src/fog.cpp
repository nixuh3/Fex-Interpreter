#include "fex.h"
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

    if (m_hadError) {
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
        m_hadError = false;
    }
}

void FexInterpreter::Run(std::string_view source) {
    Scanner scanner(source);
    const auto& tokens = scanner.ScanTokens();
    for (const auto& token : tokens) {
        std::cout << token.ToString() << "\n";
    }
}

void FexInterpreter::Report(int line, std::string_view where, std::string_view msg) {
    std::cout << "[line " << line << "] Error" << where << ": " << msg << "\n";
}

} // namespace fex

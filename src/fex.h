#pragma once

#include "arena.h"
#include "interpreter.h"
#include <string_view>

namespace fex {

struct Token;
struct RuntimeError;

class FexInterpreter {
  public:
    static void RunFile(const std::string& path);
    static void RunREPL();

    static void Error(int line, std::string_view message);
    static void Error(const Token& token, std::string_view message);
    static void RuntimeError_(const RuntimeError& error);

  private:
    FexInterpreter() {}

    static void Run(std::string_view source);

    static void Report(int line, std::string_view where, std::string_view msg);

    inline static bool s_hadError = false;
    inline static bool s_hadRuntimeError = false;
    inline static Arena s_arena{ 4 * 1024 * 1024 };

    inline static Interpreter s_interpreter;
};

} // namespace fex

#pragma once

#include "arena.h"
#include <string_view>

namespace fex {

class FexInterpreter {
  public:
    static void RunFile(const std::string& path);
    static void RunREPL();

    static void Report(int line, std::string_view where, std::string_view msg);

  private:
    FexInterpreter() {}

    static void Run(std::string_view source);

    inline static bool s_hadError = false;
    inline static Arena s_arena{ 4 * 1024 * 1024 };
};

} // namespace fex

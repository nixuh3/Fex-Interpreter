#pragma once

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

    inline static bool m_hadError = false;
};

} // namespace fex

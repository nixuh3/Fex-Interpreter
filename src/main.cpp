#include "fex.h"
#include <iostream>

int main(int argc, char const** argv) {
    if (argc > 2) {
        std::cout << "Usage: fex [path]\n";
        return 1;
    } else if (argc == 2) {
        fex::FexInterpreter::RunFile(argv[1]);
    } else {
        fex::FexInterpreter::RunREPL();
    }

    return 0;
}

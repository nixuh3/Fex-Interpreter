#include "utils.h"
#include <cmath>
#include <iomanip>
#include <sstream>

namespace fex {

std::string FormatDouble(double x, int precision) {
    if (std::isnan(x)) {
        return "nan";
    } else if (std::isinf(x)) {
        return x > 0 ? "inf" : "-inf";
    }

    std::ostringstream oss;
    oss << std::setprecision(precision) << x;

    std::string s = oss.str();

    auto pos = s.find_last_not_of('0');
    if (pos != std::string::npos) {
        s.erase(pos + 1);
    }

    if (!s.empty() && s.back() == '.') {
        s.pop_back();
    }

    return s;
}

} // namespace fex
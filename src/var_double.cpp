
#include "var_double.h"
namespace Calamity {

    const Double & Double::nan() {
        static Double nanDouble = std::numeric_limits<double>::quiet_NaN();
        return nanDouble;

    }

    const Double & Double::infinity() {
        static Double infinityDouble = std::numeric_limits<double>::infinity();
        return infinityDouble;
    }
}
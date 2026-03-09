/// @file math.cpp
#include "df/math.hpp"
#include <cmath>

namespace df {

// Pre-computed sine table (Q16.16) for 0-359 degrees.
static std::int32_t sin_table[360];
static bool sin_table_init = false;

static void init_sin_table() {
    if (sin_table_init) return;
    for (int i = 0; i < 360; ++i) {
        double rad = static_cast<double>(i) * 3.14159265358979323846 / 180.0;
        sin_table[i] = static_cast<std::int32_t>(std::sin(rad) * 65536.0);
    }
    sin_table_init = true;
}

Fixed sin_deg(int degrees) noexcept {
    init_sin_table();
    degrees = ((degrees % 360) + 360) % 360;
    return Fixed{sin_table[degrees]};
}

Fixed cos_deg(int degrees) noexcept {
    return sin_deg(degrees + 90);
}

} // namespace df

#pragma once
/// @file tape.hpp
/// Linear tape widget (airspeed, altitude, vertical speed).

#include "df/math.hpp"
#include "df/renderer.hpp"

namespace df {

struct TapeConfig {
    int min_value;       ///< Minimum displayable value
    int max_value;       ///< Maximum displayable value
    int major_tick;      ///< Major tick interval
    int minor_tick;      ///< Minor tick interval (0 = no minor ticks)
    int pixels_per_unit; ///< Pixels per value unit (scrolling scale)
    bool left_aligned;   ///< true = ticks on right side, false = ticks on left
    Color fg;
    Color bg;
};

/// Render a linear tape display.
void draw_tape(Renderer &r, Rect bounds, const TapeConfig &cfg, int value) noexcept;

} // namespace df

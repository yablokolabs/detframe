#pragma once
/// @file compass.hpp
/// Compass rose / heading indicator widget.

#include "df/math.hpp"
#include "df/renderer.hpp"

namespace df {

/// Render a compass rose at the top of the display.
/// heading: 0-359 degrees (0 = North).
void draw_compass(Renderer &r, Rect bounds, int heading, Color fg, Color bg) noexcept;

} // namespace df

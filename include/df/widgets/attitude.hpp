#pragma once
/// @file attitude.hpp
/// Attitude indicator (artificial horizon) widget.

#include "df/math.hpp"
#include "df/renderer.hpp"

namespace df {

struct AttitudeState {
    int pitch_deg; ///< Nose up positive, -90 to +90
    int roll_deg;  ///< Right wing down positive, -180 to +180
};

/// Render an attitude indicator.
void draw_attitude(Renderer &r, Rect bounds, const AttitudeState &state) noexcept;

} // namespace df

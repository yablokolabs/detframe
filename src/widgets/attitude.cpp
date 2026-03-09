#include <cstdio>
/// @file attitude.cpp
/// Attitude indicator (artificial horizon).
#include "df/widgets/attitude.hpp"
#include "df/widgets/text.hpp"

namespace df {

void draw_attitude(Renderer& r, Rect bounds, const AttitudeState& state) noexcept {
    auto cx = bounds.x + bounds.w / 2;
    auto cy = bounds.y + bounds.h / 2;

    // Sky (top half) and ground (bottom half), shifted by pitch
    int pitch_px = state.pitch_deg * 2; // 2 pixels per degree
    int horizon_y = cy - pitch_px;

    // Sky
    r.fill_rect({bounds.x, bounds.y, bounds.w, clamp(horizon_y - bounds.y, 0, bounds.h)},
                Color::rgba(30, 80, 180));
    // Ground
    if (horizon_y < bounds.y + bounds.h) {
        r.fill_rect({bounds.x, clamp(horizon_y, bounds.y, bounds.y + bounds.h),
                     bounds.w, bounds.y + bounds.h - clamp(horizon_y, bounds.y, bounds.y + bounds.h)},
                    Color::rgba(120, 80, 30));
    }

    // Horizon line
    r.thick_line(bounds.x, horizon_y, bounds.x + bounds.w, horizon_y, 2, Color::white());

    // Pitch ladder (every 10 degrees)
    for (int p = -30; p <= 30; p += 10) {
        if (p == 0) continue;
        int py = cy - (state.pitch_deg + p) * 2;
        if (py < bounds.y || py > bounds.y + bounds.h) continue;
        int half_w = (p % 20 == 0) ? 40 : 20;
        r.hline(cx - half_w, cx + half_w, py, Color::white());

        char buf[8];
        std::snprintf(buf, sizeof(buf), "%d", p > 0 ? p : -p);
        draw_text(r, cx + half_w + 4, py - 4, buf, Color::white(), 1);
    }

    // Aircraft reference (center cross)
    r.thick_line(cx - 30, cy, cx - 10, cy, 2, Color::yellow());
    r.thick_line(cx + 10, cy, cx + 30, cy, 2, Color::yellow());
    r.fill_rect({cx - 2, cy - 2, 4, 4}, Color::yellow());

    // Roll indicator arc
    r.arc(cx, cy, bounds.w / 2 - 10, -60, 60, Color::white());

    // Border
    r.rect(bounds, Color::white());
}

} // namespace df

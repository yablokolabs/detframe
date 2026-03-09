/// @file compass.cpp
/// Compass rose / heading indicator.
#include "df/widgets/compass.hpp"
#include "df/widgets/text.hpp"
#include <cstdio>

namespace df {

void draw_compass(Renderer &r, Rect bounds, int heading, Color fg, Color bg) noexcept {
    r.fill_rect(bounds, bg);

    int cx = bounds.x + bounds.w / 2;
    int y = bounds.y;
    int pixels_per_deg = 3; // 3 pixels per degree

    heading = ((heading % 360) + 360) % 360;

    // Draw tick marks across the visible range
    int visible_half = bounds.w / 2 / pixels_per_deg;

    for (int offset = -visible_half - 10; offset <= visible_half + 10; ++offset) {
        int deg = ((heading + offset) % 360 + 360) % 360;
        int px = cx + offset * pixels_per_deg;

        if (px < bounds.x || px > bounds.x + bounds.w) continue;

        if (deg % 30 == 0) {
            // Major tick
            r.vline(px, y + 2, y + 14, fg);

            // Cardinal/intercardinal labels
            const char *label = nullptr;
            if (deg == 0)
                label = "N";
            else if (deg == 90)
                label = "E";
            else if (deg == 180)
                label = "S";
            else if (deg == 270)
                label = "W";
            else {
                static char buf[4];
                std::snprintf(buf, sizeof(buf), "%d", deg / 10);
                label = buf;
            }
            if (label) {
                draw_text_center(r, px, y + 16, label, fg, 1);
            }
        } else if (deg % 10 == 0) {
            // Minor tick
            r.vline(px, y + 6, y + 14, fg);
        } else if (deg % 5 == 0) {
            // Small tick
            r.vline(px, y + 10, y + 14, fg);
        }
    }

    // Current heading pointer (triangle at center)
    r.fill_triangle(cx, y + bounds.h - 2, cx - 5, y + bounds.h - 10, cx + 5, y + bounds.h - 10, fg);

    // Current heading readout
    {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%03d", heading);
        int box_w = 32;
        int box_x = cx - box_w / 2;
        r.fill_rect({box_x, y + bounds.h - 12, box_w, 12}, Color::black());
        r.rect({box_x, y + bounds.h - 12, box_w, 12}, fg);
        draw_text_center(r, cx, y + bounds.h - 10, buf, fg, 1);
    }

    r.rect(bounds, fg);
}

} // namespace df

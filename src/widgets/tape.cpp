/// @file tape.cpp
/// Linear tape widget (airspeed/altitude display).
#include "df/widgets/tape.hpp"
#include "df/widgets/text.hpp"
#include <cstdio>

namespace df {

void draw_tape(Renderer& r, Rect bounds, const TapeConfig& cfg, int value) noexcept {
    // Background
    r.fill_rect(bounds, cfg.bg);

    int cy = bounds.y + bounds.h / 2;
    int tick_x = cfg.left_aligned ? bounds.x + bounds.w - 1 : bounds.x;

    // Draw ticks and labels
    int visible_range = bounds.h / 2 / cfg.pixels_per_unit;

    for (int v = value - visible_range - cfg.major_tick;
         v <= value + visible_range + cfg.major_tick;
         v += (cfg.minor_tick > 0 ? cfg.minor_tick : cfg.major_tick)) {

        if (v < cfg.min_value || v > cfg.max_value) continue;

        int py = cy - (v - value) * cfg.pixels_per_unit;
        if (py < bounds.y || py > bounds.y + bounds.h) continue;

        bool is_major = (v % cfg.major_tick) == 0;
        int tick_len = is_major ? 12 : 6;

        if (cfg.left_aligned) {
            r.hline(tick_x - tick_len, tick_x, py, cfg.fg);
        } else {
            r.hline(tick_x, tick_x + tick_len, py, cfg.fg);
        }

        if (is_major) {
            char buf[16];
            std::snprintf(buf, sizeof(buf), "%d", v);
            if (cfg.left_aligned) {
                draw_text_right(r, tick_x - tick_len - 4, py - 4, buf, cfg.fg, 1);
            } else {
                draw_text(r, tick_x + tick_len + 4, py - 4, buf, cfg.fg, 1);
            }
        }
    }

    // Current value box
    {
        int box_w = 56;
        int box_h = 18;
        int box_x = cfg.left_aligned ? bounds.x + bounds.w - box_w - 2 : bounds.x + 2;
        int box_y = cy - box_h / 2;

        r.fill_rect({box_x, box_y, box_w, box_h}, Color::black());
        r.rect({box_x, box_y, box_w, box_h}, cfg.fg);

        char buf[16];
        std::snprintf(buf, sizeof(buf), "%d", value);
        draw_text_center(r, box_x + box_w / 2, box_y + 5, buf, cfg.fg, 1);
    }

    // Border
    r.rect(bounds, cfg.fg);
}

} // namespace df

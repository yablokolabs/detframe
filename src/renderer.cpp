/// @file renderer.cpp
/// Software rasterizer — all integer math, fully deterministic.
#include "df/renderer.hpp"
#include <algorithm>
#include <cstdlib>

namespace df {

void Renderer::hline(int x0, int x1, int y, Color c) noexcept {
    if (x0 > x1) std::swap(x0, x1);
    for (int x = x0; x <= x1; ++x) fb_.set_pixel(x, y, c);
}

void Renderer::vline(int x, int y0, int y1, Color c) noexcept {
    if (y0 > y1) std::swap(y0, y1);
    for (int y = y0; y <= y1; ++y) fb_.set_pixel(x, y, c);
}

void Renderer::line(int x0, int y0, int x1, int y1, Color c) noexcept {
    // Bresenham's line algorithm (deterministic)
    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    for (;;) {
        fb_.set_pixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Renderer::thick_line(int x0, int y0, int x1, int y1, int thickness, Color c) noexcept {
    int half = thickness / 2;
    for (int d = -half; d <= half; ++d) {
        if (std::abs(y1 - y0) > std::abs(x1 - x0)) {
            line(x0 + d, y0, x1 + d, y1, c);
        } else {
            line(x0, y0 + d, x1, y1 + d, c);
        }
    }
}

void Renderer::rect(Rect r, Color c) noexcept {
    hline(r.x, r.x + r.w - 1, r.y, c);
    hline(r.x, r.x + r.w - 1, r.y + r.h - 1, c);
    vline(r.x, r.y, r.y + r.h - 1, c);
    vline(r.x + r.w - 1, r.y, r.y + r.h - 1, c);
}

void Renderer::fill_rect(Rect r, Color c) noexcept {
    fb_.fill_rect(r, c);
}

void Renderer::fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color c) noexcept {
    // Sort vertices by y
    if (y0 > y1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    if (y0 > y2) {
        std::swap(x0, x2);
        std::swap(y0, y2);
    }
    if (y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    if (y2 == y0) return; // degenerate

    // Scanline fill (integer math only)
    for (int y = y0; y <= y2; ++y) {
        int xa, xb;
        // Edge 0→2 is always active
        xa = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        if (y < y1) {
            if (y1 == y0)
                xb = x0;
            else
                xb = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        } else {
            if (y2 == y1)
                xb = x1;
            else
                xb = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        }
        hline(std::min(xa, xb), std::max(xa, xb), y, c);
    }
}

void Renderer::circle(int cx, int cy, int radius, Color c) noexcept {
    // Midpoint circle algorithm (deterministic)
    int x = radius, y = 0, d = 1 - radius;
    while (x >= y) {
        fb_.set_pixel(cx + x, cy + y, c);
        fb_.set_pixel(cx - x, cy + y, c);
        fb_.set_pixel(cx + x, cy - y, c);
        fb_.set_pixel(cx - x, cy - y, c);
        fb_.set_pixel(cx + y, cy + x, c);
        fb_.set_pixel(cx - y, cy + x, c);
        fb_.set_pixel(cx + y, cy - x, c);
        fb_.set_pixel(cx - y, cy - x, c);
        ++y;
        if (d <= 0) {
            d += 2 * y + 1;
        } else {
            --x;
            d += 2 * (y - x) + 1;
        }
    }
}

void Renderer::fill_circle(int cx, int cy, int radius, Color c) noexcept {
    int x = radius, y = 0, d = 1 - radius;
    while (x >= y) {
        hline(cx - x, cx + x, cy + y, c);
        hline(cx - x, cx + x, cy - y, c);
        hline(cx - y, cx + y, cy + x, c);
        hline(cx - y, cx + y, cy - x, c);
        ++y;
        if (d <= 0) {
            d += 2 * y + 1;
        } else {
            --x;
            d += 2 * (y - x) + 1;
        }
    }
}

void Renderer::arc(int cx, int cy, int radius, int start_deg, int end_deg, Color c) noexcept {
    start_deg = ((start_deg % 360) + 360) % 360;
    end_deg = ((end_deg % 360) + 360) % 360;
    for (int d = start_deg; d != end_deg; d = (d + 1) % 360) {
        int px = cx + (sin_deg(d) * Fixed::from_int(radius)).to_int();
        int py = cy - (cos_deg(d) * Fixed::from_int(radius)).to_int();
        fb_.set_pixel(px, py, c);
    }
}

} // namespace df

#pragma once
/// @file renderer.hpp
/// Software rasterizer: lines, filled rectangles, filled triangles, circles.
/// All operations are deterministic (no floating-point in rasterization).

#include "df/framebuffer.hpp"
#include "df/math.hpp"

namespace df {

class Renderer {
  public:
    explicit Renderer(Framebuffer &fb) noexcept : fb_(fb) {}

    /// Draw a horizontal line.
    void hline(int x0, int x1, int y, Color c) noexcept;

    /// Draw a vertical line.
    void vline(int x, int y0, int y1, Color c) noexcept;

    /// Draw an arbitrary line (Bresenham's algorithm — deterministic).
    void line(int x0, int y0, int x1, int y1, Color c) noexcept;

    /// Draw a line with thickness.
    void thick_line(int x0, int y0, int x1, int y1, int thickness, Color c) noexcept;

    /// Draw a rectangle outline.
    void rect(Rect r, Color c) noexcept;

    /// Fill a rectangle.
    void fill_rect(Rect r, Color c) noexcept;

    /// Draw a filled triangle (scanline rasterization — deterministic).
    void fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color c) noexcept;

    /// Draw a circle outline (Midpoint algorithm — deterministic).
    void circle(int cx, int cy, int radius, Color c) noexcept;

    /// Draw a filled circle.
    void fill_circle(int cx, int cy, int radius, Color c) noexcept;

    /// Draw an arc (start_deg to end_deg, integer degrees).
    void arc(int cx, int cy, int radius, int start_deg, int end_deg, Color c) noexcept;

    /// Get underlying framebuffer.
    [[nodiscard]] Framebuffer &framebuffer() noexcept { return fb_; }

  private:
    Framebuffer &fb_;
};

} // namespace df

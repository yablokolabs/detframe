#pragma once
/// @file text.hpp
/// Bitmap font text rendering (built-in 8x8 font).
/// Deterministic: no FreeType, no system fonts, no floating point.

#include "df/math.hpp"
#include "df/renderer.hpp"

namespace df {

/// Draw a string using the built-in 8x8 bitmap font.
/// scale: pixel multiplier (1 = 8px tall, 2 = 16px tall, etc.)
void draw_text(Renderer &r, int x, int y, const char *text, Color c, int scale = 1) noexcept;

/// Draw a right-aligned string.
void draw_text_right(Renderer &r, int x, int y, const char *text, Color c, int scale = 1) noexcept;

/// Draw a centered string.
void draw_text_center(Renderer &r, int x, int y, const char *text, Color c, int scale = 1) noexcept;

/// Get text width in pixels for given scale.
[[nodiscard]] int text_width(const char *text, int scale = 1) noexcept;

} // namespace df

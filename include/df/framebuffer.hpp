#pragma once
/// @file framebuffer.hpp
/// RGBA8888 pixel buffer with deterministic operations.

#include "df/math.hpp"
#include <cstdint>
#include <cstring>
#include <vector>

namespace df {

class Framebuffer {
  public:
    Framebuffer(int width, int height) noexcept;

    /// Clear entire buffer to a color.
    void clear(Color c) noexcept;

    /// Set a single pixel (bounds-checked).
    void set_pixel(int x, int y, Color c) noexcept;

    /// Get a pixel (bounds-checked, returns transparent if OOB).
    [[nodiscard]] Color get_pixel(int x, int y) const noexcept;

    /// Alpha-blend a pixel on top of existing content.
    void blend_pixel(int x, int y, Color c) noexcept;

    /// Fill a rectangle with a solid color.
    void fill_rect(Rect r, Color c) noexcept;

    /// Raw pixel data (RGBA8888, row-major).
    [[nodiscard]] const std::uint8_t *data() const noexcept { return pixels_.data(); }
    [[nodiscard]] std::uint8_t *data() noexcept { return pixels_.data(); }

    [[nodiscard]] int width() const noexcept { return width_; }
    [[nodiscard]] int height() const noexcept { return height_; }
    [[nodiscard]] std::size_t stride() const noexcept {
        return static_cast<std::size_t>(width_) * 4;
    }
    [[nodiscard]] std::size_t size_bytes() const noexcept { return pixels_.size(); }

    /// Compare two framebuffers for bit-exact equality.
    [[nodiscard]] bool equals(const Framebuffer &other) const noexcept;

    /// Count differing pixels between two framebuffers.
    [[nodiscard]] std::size_t diff_count(const Framebuffer &other) const noexcept;

  private:
    int width_, height_;
    std::vector<std::uint8_t> pixels_; // RGBA8888
};

} // namespace df

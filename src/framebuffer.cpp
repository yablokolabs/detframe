/// @file framebuffer.cpp
#include "df/framebuffer.hpp"

namespace df {

Framebuffer::Framebuffer(int width, int height) noexcept
    : width_(width), height_(height),
      pixels_(static_cast<std::size_t>(width) * height * 4, 0) {}

void Framebuffer::clear(Color c) noexcept {
    for (std::size_t i = 0; i < pixels_.size(); i += 4) {
        pixels_[i]     = c.r;
        pixels_[i + 1] = c.g;
        pixels_[i + 2] = c.b;
        pixels_[i + 3] = c.a;
    }
}

void Framebuffer::set_pixel(int x, int y, Color c) noexcept {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
    auto off = (static_cast<std::size_t>(y) * width_ + x) * 4;
    pixels_[off]     = c.r;
    pixels_[off + 1] = c.g;
    pixels_[off + 2] = c.b;
    pixels_[off + 3] = c.a;
}

Color Framebuffer::get_pixel(int x, int y) const noexcept {
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return Color::transparent();
    auto off = (static_cast<std::size_t>(y) * width_ + x) * 4;
    return {pixels_[off], pixels_[off + 1], pixels_[off + 2], pixels_[off + 3]};
}

void Framebuffer::blend_pixel(int x, int y, Color c) noexcept {
    if (c.a == 0) return;
    if (c.a == 255) { set_pixel(x, y, c); return; }
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return;

    auto off = (static_cast<std::size_t>(y) * width_ + x) * 4;
    auto sa = static_cast<int>(c.a);
    auto da = 255 - sa;

    pixels_[off]     = static_cast<std::uint8_t>((c.r * sa + pixels_[off] * da) / 255);
    pixels_[off + 1] = static_cast<std::uint8_t>((c.g * sa + pixels_[off + 1] * da) / 255);
    pixels_[off + 2] = static_cast<std::uint8_t>((c.b * sa + pixels_[off + 2] * da) / 255);
    pixels_[off + 3] = 255;
}

void Framebuffer::fill_rect(Rect r, Color c) noexcept {
    int x0 = clamp(r.x, 0, width_);
    int y0 = clamp(r.y, 0, height_);
    int x1 = clamp(r.x + r.w, 0, width_);
    int y1 = clamp(r.y + r.h, 0, height_);

    for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x)
            set_pixel(x, y, c);
}

bool Framebuffer::equals(const Framebuffer& other) const noexcept {
    if (width_ != other.width_ || height_ != other.height_) return false;
    return std::memcmp(pixels_.data(), other.pixels_.data(), pixels_.size()) == 0;
}

std::size_t Framebuffer::diff_count(const Framebuffer& other) const noexcept {
    if (width_ != other.width_ || height_ != other.height_)
        return static_cast<std::size_t>(width_ * height_);
    std::size_t count = 0;
    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (std::memcmp(&pixels_[(y * width_ + x) * 4],
                           &other.pixels_[(y * width_ + x) * 4], 4) != 0)
                ++count;
    return count;
}

} // namespace df

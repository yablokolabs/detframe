#pragma once
/// @file math.hpp
/// Fixed-point and floating-point math for deterministic rendering.
/// Q16.16 fixed-point ensures bit-exact results across compilers.

#include <cmath>
#include <cstdint>

namespace df {

/// Q16.16 fixed-point number.
struct Fixed {
    std::int32_t raw;

    constexpr Fixed() noexcept : raw(0) {}
    constexpr explicit Fixed(std::int32_t r) noexcept : raw(r) {}

    static constexpr Fixed from_int(int v) noexcept { return Fixed{v << 16}; }
    static constexpr Fixed from_float(float v) noexcept {
        return Fixed{static_cast<std::int32_t>(v * 65536.0f)};
    }

    [[nodiscard]] constexpr int to_int() const noexcept { return raw >> 16; }
    [[nodiscard]] constexpr float to_float() const noexcept {
        return static_cast<float>(raw) / 65536.0f;
    }

    constexpr Fixed operator+(Fixed o) const noexcept { return Fixed{raw + o.raw}; }
    constexpr Fixed operator-(Fixed o) const noexcept { return Fixed{raw - o.raw}; }
    constexpr Fixed operator*(Fixed o) const noexcept {
        return Fixed{static_cast<std::int32_t>((static_cast<std::int64_t>(raw) * o.raw) >> 16)};
    }
    constexpr Fixed operator/(Fixed o) const noexcept {
        return Fixed{static_cast<std::int32_t>((static_cast<std::int64_t>(raw) << 16) / o.raw)};
    }
    constexpr bool operator<(Fixed o) const noexcept { return raw < o.raw; }
    constexpr bool operator>(Fixed o) const noexcept { return raw > o.raw; }
    constexpr bool operator==(Fixed o) const noexcept { return raw == o.raw; }
    constexpr Fixed operator-() const noexcept { return Fixed{-raw}; }
};

/// Integer 2D point.
struct Point {
    int x, y;
};

/// Integer rectangle.
struct Rect {
    int x, y, w, h;

    [[nodiscard]] constexpr bool contains(int px, int py) const noexcept {
        return px >= x && px < x + w && py >= y && py < y + h;
    }
};

/// RGBA color (8 bits per channel).
struct Color {
    std::uint8_t r, g, b, a;

    static constexpr Color rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b,
                                std::uint8_t a = 255) noexcept {
        return {r, g, b, a};
    }

    static constexpr Color white() noexcept { return {255, 255, 255, 255}; }
    static constexpr Color black() noexcept { return {0, 0, 0, 255}; }
    static constexpr Color green() noexcept { return {0, 255, 0, 255}; }
    static constexpr Color cyan() noexcept { return {0, 255, 255, 255}; }
    static constexpr Color yellow() noexcept { return {255, 255, 0, 255}; }
    static constexpr Color red() noexcept { return {255, 0, 0, 255}; }
    static constexpr Color transparent() noexcept { return {0, 0, 0, 0}; }
};

/// Lookup table sin (deterministic, integer degrees).
/// Returns Fixed Q16.16.
Fixed sin_deg(int degrees) noexcept;

/// Lookup table cos (deterministic, integer degrees).
Fixed cos_deg(int degrees) noexcept;

/// Clamp integer to range.
[[nodiscard]] constexpr int clamp(int v, int lo, int hi) noexcept {
    return v < lo ? lo : (v > hi ? hi : v);
}

} // namespace df

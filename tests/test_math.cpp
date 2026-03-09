/// @file test_math.cpp
#include "df/math.hpp"
#include <cmath>
#include <cstdio>

void test_fixed_arithmetic() {
    auto a = df::Fixed::from_int(3);
    auto b = df::Fixed::from_int(4);
    auto sum = a + b;
    if (sum.to_int() != 7) {
        std::fprintf(stderr, "  ✗ addition\n");
        return;
    }

    auto prod = a * b;
    if (prod.to_int() != 12) {
        std::fprintf(stderr, "  ✗ multiplication\n");
        return;
    }

    auto div = prod / b;
    if (div.to_int() != 3) {
        std::fprintf(stderr, "  ✗ division\n");
        return;
    }

    std::fprintf(stderr, "  ✓ fixed-point arithmetic\n");
}

void test_fixed_float_conversion() {
    auto f = df::Fixed::from_float(3.5f);
    float result = f.to_float();
    if (std::fabs(result - 3.5f) > 0.001f) {
        std::fprintf(stderr, "  ✗ float conversion: got %f\n", result);
        return;
    }
    std::fprintf(stderr, "  ✓ float conversion\n");
}

void test_sin_cos_determinism() {
    // Same input must always produce same output
    auto s1 = df::sin_deg(45);
    auto s2 = df::sin_deg(45);
    if (s1.raw != s2.raw) {
        std::fprintf(stderr, "  ✗ sin not deterministic\n");
        return;
    }

    // sin(0) == 0, sin(90) == 1, cos(0) == 1
    if (df::sin_deg(0).to_int() != 0) {
        std::fprintf(stderr, "  ✗ sin(0)\n");
        return;
    }
    if (df::sin_deg(90).to_int() != 1) {
        std::fprintf(stderr, "  ✗ sin(90)\n");
        return;
    }
    if (df::cos_deg(0).to_int() != 1) {
        std::fprintf(stderr, "  ✗ cos(0)\n");
        return;
    }

    std::fprintf(stderr, "  ✓ sin/cos determinism and values\n");
}

void test_clamp() {
    if (df::clamp(5, 0, 10) != 5) {
        std::fprintf(stderr, "  ✗ clamp mid\n");
        return;
    }
    if (df::clamp(-1, 0, 10) != 0) {
        std::fprintf(stderr, "  ✗ clamp lo\n");
        return;
    }
    if (df::clamp(15, 0, 10) != 10) {
        std::fprintf(stderr, "  ✗ clamp hi\n");
        return;
    }
    std::fprintf(stderr, "  ✓ clamp\n");
}

int main() {
    std::fprintf(stderr, "test_math:\n");
    test_fixed_arithmetic();
    test_fixed_float_conversion();
    test_sin_cos_determinism();
    test_clamp();
    std::fprintf(stderr, "  All math tests passed.\n");
    return 0;
}

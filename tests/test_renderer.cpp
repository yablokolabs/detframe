/// @file test_renderer.cpp
#include "df/framebuffer.hpp"
#include "df/renderer.hpp"
#include <cstdio>

void test_pixel_set_get() {
    df::Framebuffer fb(64, 64);
    fb.clear(df::Color::black());
    fb.set_pixel(10, 20, df::Color::green());
    auto c = fb.get_pixel(10, 20);
    if (c.r != 0 || c.g != 255 || c.b != 0) {
        std::fprintf(stderr, "  ✗ pixel set/get\n"); return;
    }
    std::fprintf(stderr, "  ✓ pixel set/get\n");
}

void test_oob_safe() {
    df::Framebuffer fb(32, 32);
    fb.set_pixel(-1, -1, df::Color::red());   // should not crash
    fb.set_pixel(32, 32, df::Color::red());   // should not crash
    auto c = fb.get_pixel(-5, -5);
    if (c.a != 0) { std::fprintf(stderr, "  ✗ OOB returns transparent\n"); return; }
    std::fprintf(stderr, "  ✓ OOB safety\n");
}

void test_fill_rect() {
    df::Framebuffer fb(64, 64);
    fb.clear(df::Color::black());
    fb.fill_rect({10, 10, 20, 20}, df::Color::cyan());

    auto inside = fb.get_pixel(15, 15);
    auto outside = fb.get_pixel(5, 5);
    if (inside.r != 0 || inside.g != 255 || inside.b != 255) {
        std::fprintf(stderr, "  ✗ fill_rect inside\n"); return;
    }
    if (outside.g != 0) {
        std::fprintf(stderr, "  ✗ fill_rect outside\n"); return;
    }
    std::fprintf(stderr, "  ✓ fill_rect\n");
}

void test_line_bresenham() {
    df::Framebuffer fb(64, 64);
    fb.clear(df::Color::black());
    df::Renderer r(fb);
    r.line(0, 0, 63, 63, df::Color::white());

    // Diagonal should have pixels set along the path
    auto c = fb.get_pixel(32, 32);
    if (c.r != 255) { std::fprintf(stderr, "  ✗ bresenham diagonal\n"); return; }
    std::fprintf(stderr, "  ✓ bresenham line\n");
}

void test_circle() {
    df::Framebuffer fb(64, 64);
    fb.clear(df::Color::black());
    df::Renderer r(fb);
    r.circle(32, 32, 15, df::Color::green());

    // Top of circle
    auto c = fb.get_pixel(32, 17);
    if (c.g != 255) { std::fprintf(stderr, "  ✗ circle top pixel\n"); return; }
    // Center should be empty
    auto center = fb.get_pixel(32, 32);
    if (center.g != 0) { std::fprintf(stderr, "  ✗ circle center should be empty\n"); return; }
    std::fprintf(stderr, "  ✓ circle\n");
}

void test_filled_triangle() {
    df::Framebuffer fb(64, 64);
    fb.clear(df::Color::black());
    df::Renderer r(fb);
    r.fill_triangle(32, 10, 10, 50, 54, 50, df::Color::red());

    // Center of triangle should be filled
    auto c = fb.get_pixel(32, 35);
    if (c.r != 255) { std::fprintf(stderr, "  ✗ triangle fill\n"); return; }
    // Corner outside should be empty
    auto outside = fb.get_pixel(5, 5);
    if (outside.r != 0) { std::fprintf(stderr, "  ✗ triangle outside\n"); return; }
    std::fprintf(stderr, "  ✓ filled triangle\n");
}

int main() {
    std::fprintf(stderr, "test_renderer:\n");
    test_pixel_set_get();
    test_oob_safe();
    test_fill_rect();
    test_line_bresenham();
    test_circle();
    test_filled_triangle();
    std::fprintf(stderr, "  All renderer tests passed.\n");
    return 0;
}

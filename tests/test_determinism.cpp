/// @file test_determinism.cpp
/// Core property: rendering the same frame twice must produce bit-identical output.
#include "df/framebuffer.hpp"
#include "df/renderer.hpp"
#include "df/widgets/attitude.hpp"
#include "df/widgets/compass.hpp"
#include "df/widgets/tape.hpp"
#include "df/widgets/text.hpp"
#include <cstdio>

static void render_pfd_frame(df::Renderer &r, int w, int h, int frame) {
    auto &fb = r.framebuffer();
    fb.clear(df::Color::rgba(10, 10, 15));

    int pitch = (frame / 3) % 40 - 20;
    int roll = (frame / 2) % 60 - 30;
    int airspeed = 220 + (frame % 40) - 20;
    int altitude = 12000 + (frame % 100) * 10;
    int heading = (frame * 1) % 360;

    int att_w = w * 40 / 100;
    int att_h = h * 70 / 100;
    int att_x = (w - att_w) / 2;
    int att_y = 40;
    int tape_w = 80;

    df::AttitudeState att{pitch, roll};
    draw_attitude(r, {att_x, att_y, att_w, att_h}, att);

    df::TapeConfig spd{};
    spd.min_value = 0;
    spd.max_value = 500;
    spd.major_tick = 20;
    spd.minor_tick = 10;
    spd.pixels_per_unit = 3;
    spd.left_aligned = true;
    spd.fg = df::Color::green();
    spd.bg = df::Color::rgba(10, 10, 10);
    draw_tape(r, {att_x - tape_w - 10, att_y, tape_w, att_h}, spd, airspeed);

    df::TapeConfig alt{};
    alt.min_value = 0;
    alt.max_value = 50000;
    alt.major_tick = 500;
    alt.minor_tick = 100;
    alt.pixels_per_unit = 1;
    alt.left_aligned = false;
    alt.fg = df::Color::green();
    alt.bg = df::Color::rgba(10, 10, 10);
    draw_tape(r, {att_x + att_w + 10, att_y, tape_w, att_h}, alt, altitude);

    draw_compass(r, {att_x, 2, att_w, 36}, heading, df::Color::green(),
                 df::Color::rgba(10, 10, 10));

    draw_text(r, 4, 2, "DETFRAME PFD", df::Color::rgba(100, 100, 100), 1);
}

void test_determinism_single_frame() {
    df::Framebuffer fb1(640, 480);
    df::Framebuffer fb2(640, 480);
    df::Renderer r1(fb1), r2(fb2);

    render_pfd_frame(r1, 640, 480, 42);
    render_pfd_frame(r2, 640, 480, 42);

    if (!fb1.equals(fb2)) {
        auto diff = fb1.diff_count(fb2);
        std::fprintf(stderr, "  ✗ single frame determinism: %zu pixels differ\n", diff);
        return;
    }
    std::fprintf(stderr, "  ✓ single frame determinism (bit-exact)\n");
}

void test_determinism_sequence() {
    // Render frames 0-49 twice, compare each pair
    df::Framebuffer fb1(640, 480), fb2(640, 480);
    df::Renderer r1(fb1), r2(fb2);

    for (int f = 0; f < 50; ++f) {
        render_pfd_frame(r1, 640, 480, f);
        render_pfd_frame(r2, 640, 480, f);

        if (!fb1.equals(fb2)) {
            std::fprintf(stderr, "  ✗ sequence determinism: frame %d differs\n", f);
            return;
        }
    }
    std::fprintf(stderr, "  ✓ sequence determinism (50 frames, all bit-exact)\n");
}

void test_different_frames_differ() {
    df::Framebuffer fb1(640, 480), fb2(640, 480);
    df::Renderer r1(fb1), r2(fb2);

    render_pfd_frame(r1, 640, 480, 0);
    render_pfd_frame(r2, 640, 480, 25);

    if (fb1.equals(fb2)) {
        std::fprintf(stderr, "  ✗ different frames should produce different output\n");
        return;
    }
    std::fprintf(stderr, "  ✓ different frames produce different output\n");
}

int main() {
    std::fprintf(stderr, "test_determinism:\n");
    test_determinism_single_frame();
    test_determinism_sequence();
    test_different_frames_differ();
    std::fprintf(stderr, "  All determinism tests passed.\n");
    return 0;
}

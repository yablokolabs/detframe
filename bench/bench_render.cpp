/// @file bench_render.cpp
/// Measure render time for a full PFD frame.
#include "df/framebuffer.hpp"
#include "df/renderer.hpp"
#include "df/widgets/attitude.hpp"
#include "df/widgets/tape.hpp"
#include "df/widgets/compass.hpp"
#include "df/widgets/text.hpp"
#include <cstdio>
#include <cstdint>
#include <time.h>

static std::int64_t now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<std::int64_t>(ts.tv_sec) * 1'000'000'000LL + ts.tv_nsec;
}

static void render_frame(df::Renderer& r, int w, int h, int f) {
    auto& fb = r.framebuffer();
    fb.clear(df::Color::rgba(10, 10, 15));

    int att_w = w * 40 / 100, att_h = h * 70 / 100;
    int att_x = (w - att_w) / 2, att_y = 40;
    int tape_w = 80;

    df::AttitudeState att{(f / 3) % 40 - 20, (f / 2) % 60 - 30};
    draw_attitude(r, {att_x, att_y, att_w, att_h}, att);

    df::TapeConfig spd{0, 500, 20, 10, 3, true, df::Color::green(), df::Color::rgba(10,10,10)};
    draw_tape(r, {att_x - tape_w - 10, att_y, tape_w, att_h}, spd, 220 + (f % 40) - 20);

    df::TapeConfig alt{0, 50000, 500, 100, 1, false, df::Color::green(), df::Color::rgba(10,10,10)};
    draw_tape(r, {att_x + att_w + 10, att_y, tape_w, att_h}, alt, 12000 + (f % 100) * 10);

    draw_compass(r, {att_x, 2, att_w, 36}, (f * 1) % 360,
                 df::Color::green(), df::Color::rgba(10, 10, 10));
    draw_text(r, 4, 2, "DETFRAME PFD", df::Color::rgba(100,100,100), 1);
}

int main() {
    constexpr int N = 1000;
    constexpr int W = 640, H = 480;

    df::Framebuffer fb(W, H);
    df::Renderer r(fb);

    // Warmup
    for (int i = 0; i < 50; ++i) render_frame(r, W, H, i);

    auto start = now_ns();
    for (int i = 0; i < N; ++i) render_frame(r, W, H, i);
    auto elapsed = now_ns() - start;

    double avg_us = static_cast<double>(elapsed) / N / 1000.0;
    double fps = 1'000'000.0 / avg_us;

    std::fprintf(stderr,
        "PFD Render Benchmark (%dx%d, %d frames)\n"
        "  Average: %.1f μs/frame\n"
        "  Max FPS: %.0f fps\n"
        "  Budget:  %.1f%% of 16.67ms (60fps)\n",
        W, H, N, avg_us, fps, avg_us / 16670.0 * 100.0);

    return 0;
}

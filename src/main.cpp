/// @file main.cpp
/// detframe demo: renders a Primary Flight Display (PFD) and captures frames.
///
/// Usage:
///   detframe [--frames N] [--capture] [--width W] [--height H]

#include "df/capture.hpp"
#include "df/frame_timer.hpp"
#include "df/framebuffer.hpp"
#include "df/renderer.hpp"
#include "df/widgets/attitude.hpp"
#include "df/widgets/compass.hpp"
#include "df/widgets/tape.hpp"
#include "df/widgets/text.hpp"

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace df;

static volatile bool g_running = true;

static void signal_handler(int) {
    g_running = false;
}

static void render_pfd(Renderer &r, int width, int height, int frame) {
    auto &fb = r.framebuffer();
    fb.clear(Color::rgba(10, 10, 15));

    // Simulated flight data (varies with frame for animation)
    int pitch = (frame / 3) % 40 - 20;         // -20 to +20 degrees
    int roll = (frame / 2) % 60 - 30;          // -30 to +30 degrees
    int airspeed = 220 + (frame % 40) - 20;    // 200-240 kts
    int altitude = 12000 + (frame % 100) * 10; // 12000-13000 ft
    int heading = (frame * 1) % 360;           // rotating heading

    // Layout
    int att_w = width * 40 / 100;
    int att_h = height * 70 / 100;
    int att_x = (width - att_w) / 2;
    int att_y = 40;

    int tape_w = 80;
    int tape_h = att_h;

    // Attitude indicator (center)
    df::AttitudeState att_state{pitch, roll};
    draw_attitude(r, {att_x, att_y, att_w, att_h}, att_state);

    // Airspeed tape (left)
    df::TapeConfig spd_cfg{};
    spd_cfg.min_value = 0;
    spd_cfg.max_value = 500;
    spd_cfg.major_tick = 20;
    spd_cfg.minor_tick = 10;
    spd_cfg.pixels_per_unit = 3;
    spd_cfg.left_aligned = true;
    spd_cfg.fg = Color::green();
    spd_cfg.bg = Color::rgba(10, 10, 10);
    draw_tape(r, {att_x - tape_w - 10, att_y, tape_w, tape_h}, spd_cfg, airspeed);

    // Altitude tape (right)
    df::TapeConfig alt_cfg{};
    alt_cfg.min_value = 0;
    alt_cfg.max_value = 50000;
    alt_cfg.major_tick = 500;
    alt_cfg.minor_tick = 100;
    alt_cfg.pixels_per_unit = 1;
    alt_cfg.left_aligned = false;
    alt_cfg.fg = Color::green();
    alt_cfg.bg = Color::rgba(10, 10, 10);
    draw_tape(r, {att_x + att_w + 10, att_y, tape_w, tape_h}, alt_cfg, altitude);

    // Compass (top)
    draw_compass(r, {att_x, 2, att_w, 36}, heading, Color::green(), Color::rgba(10, 10, 10));

    // Title
    draw_text(r, 4, 2, "DETFRAME PFD", Color::rgba(100, 100, 100), 1);

    // Frame counter
    char buf[32];
    std::snprintf(buf, sizeof(buf), "FRM %05d", frame);
    draw_text_right(r, width - 4, 2, buf, Color::rgba(100, 100, 100), 1);
}

int main(int argc, char *argv[]) {
    int width = 640, height = 480;
    int num_frames = 100;
    bool capture = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--frames") == 0 && i + 1 < argc)
            num_frames = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--capture") == 0)
            capture = true;
        else if (std::strcmp(argv[i], "--width") == 0 && i + 1 < argc)
            width = std::atoi(argv[++i]);
        else if (std::strcmp(argv[i], "--height") == 0 && i + 1 < argc)
            height = std::atoi(argv[++i]);
    }

    std::fprintf(stderr,
                 "╔══════════════════════════════════════════════╗\n"
                 "║           detframe v0.1.0                    ║\n"
                 "║  Deterministic rendering pipeline            ║\n"
                 "╚══════════════════════════════════════════════╝\n\n"
                 "Config:\n"
                 "  Resolution: %dx%d\n"
                 "  Frames:     %d\n"
                 "  Capture:    %s\n\n",
                 width, height, num_frames, capture ? "yes" : "no");

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    Framebuffer fb(width, height);
    Renderer renderer(fb);
    FrameTimer timer(16); // 60fps target (16ms)

    for (int f = 0; f < num_frames && g_running; ++f) {
        timer.begin_frame();

        render_pfd(renderer, width, height, f);

        auto render_us = timer.end_frame();

        if (capture) {
            char path[64];
            std::snprintf(path, sizeof(path), "frame_%05d.ppm", f);
            write_ppm(fb, path);
        }

        if (f % 10 == 0) {
            std::fprintf(stderr, "  Frame %4d/%d  render=%4ldμs  %s\n", f, num_frames, render_us,
                         timer.overrun() ? "OVERRUN" : "ok");
        }
    }

    std::fprintf(stderr,
                 "\n══════════════════════════════════════════════\n"
                 "Results:\n"
                 "  Frames rendered: %lu\n"
                 "  Worst render:    %ldμs\n"
                 "  Budget overruns: %lu\n"
                 "══════════════════════════════════════════════\n",
                 timer.frame_count(), timer.worst_render_us(), timer.overrun_count());

    return 0;
}

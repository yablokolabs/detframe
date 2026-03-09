/// @file capture.cpp
#include "df/capture.hpp"
#include <cstdio>

namespace df {

bool write_ppm(const Framebuffer& fb, const char* path) noexcept {
    FILE* f = std::fopen(path, "wb");
    if (!f) return false;

    std::fprintf(f, "P6\n%d %d\n255\n", fb.width(), fb.height());

    // Write RGB (skip alpha)
    for (int y = 0; y < fb.height(); ++y) {
        for (int x = 0; x < fb.width(); ++x) {
            auto c = fb.get_pixel(x, y);
            std::uint8_t rgb[3] = {c.r, c.g, c.b};
            std::fwrite(rgb, 1, 3, f);
        }
    }

    std::fclose(f);
    return true;
}

bool compare_ppm(const char* path_a, const char* path_b) noexcept {
    FILE* fa = std::fopen(path_a, "rb");
    FILE* fb = std::fopen(path_b, "rb");
    if (!fa || !fb) {
        if (fa) std::fclose(fa);
        if (fb) std::fclose(fb);
        return false;
    }

    bool equal = true;
    int ca, cb;
    while ((ca = std::fgetc(fa)) != EOF && (cb = std::fgetc(fb)) != EOF) {
        if (ca != cb) { equal = false; break; }
    }
    // Check both reached EOF
    if (equal) {
        equal = (std::fgetc(fa) == EOF && std::fgetc(fb) == EOF);
    }

    std::fclose(fa);
    std::fclose(fb);
    return equal;
}

} // namespace df

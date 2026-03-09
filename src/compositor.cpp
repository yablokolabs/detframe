/// @file compositor.cpp
#include "df/compositor.hpp"

namespace df {

void Compositor::add_layer(Framebuffer& fb, int x, int y, std::uint8_t opacity) noexcept {
    layers_.push_back({&fb, x, y, opacity, true});
}

void Compositor::composite() noexcept {
    output_.clear(Color::black());

    for (auto& layer : layers_) {
        if (!layer.visible || !layer.fb) continue;

        auto& src = *layer.fb;
        for (int y = 0; y < src.height(); ++y) {
            for (int x = 0; x < src.width(); ++x) {
                auto c = src.get_pixel(x, y);
                if (c.a == 0) continue;

                // Apply layer opacity
                if (layer.opacity < 255) {
                    c.a = static_cast<std::uint8_t>(
                        (static_cast<int>(c.a) * layer.opacity) / 255);
                }

                output_.blend_pixel(x + layer.offset_x, y + layer.offset_y, c);
            }
        }
    }
}

} // namespace df

#pragma once
/// @file compositor.hpp
/// Layer compositor: blends multiple framebuffers into a final output.

#include "df/framebuffer.hpp"
#include <vector>

namespace df {

/// A compositing layer.
struct Layer {
    Framebuffer* fb;
    int offset_x;
    int offset_y;
    std::uint8_t opacity; ///< 0-255
    bool visible;
};

class Compositor {
public:
    explicit Compositor(Framebuffer& output) noexcept : output_(output) {}

    /// Add a layer (drawn in order: first added = bottom).
    void add_layer(Framebuffer& fb, int x = 0, int y = 0,
                   std::uint8_t opacity = 255) noexcept;

    /// Composite all visible layers into the output framebuffer.
    void composite() noexcept;

    /// Get layer count.
    [[nodiscard]] std::size_t layer_count() const noexcept { return layers_.size(); }

    /// Access a layer.
    [[nodiscard]] Layer& layer(std::size_t idx) noexcept { return layers_[idx]; }

private:
    Framebuffer& output_;
    std::vector<Layer> layers_;
};

} // namespace df

#pragma once
/// @file scene.hpp
/// Scene graph: declarative description of what to render.

#include "df/math.hpp"
#include <vector>

namespace df {

/// Scene element types.
enum class ElementType : std::uint8_t {
    Line,
    Rect,
    FilledRect,
    Circle,
    FilledCircle,
    Triangle,
    Text,
};

/// A single scene element.
struct SceneElement {
    ElementType type;
    int x0, y0, x1, y1, x2, y2; // coordinates (usage depends on type)
    int radius;
    Color color;
    const char *text; // for Text elements
    int font_scale;
};

class Scene {
  public:
    void add_line(int x0, int y0, int x1, int y1, Color c) noexcept;
    void add_rect(int x, int y, int w, int h, Color c) noexcept;
    void add_filled_rect(int x, int y, int w, int h, Color c) noexcept;
    void add_circle(int cx, int cy, int r, Color c) noexcept;
    void add_filled_circle(int cx, int cy, int r, Color c) noexcept;
    void add_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color c) noexcept;
    void add_text(int x, int y, const char *text, Color c, int scale = 1) noexcept;

    [[nodiscard]] const std::vector<SceneElement> &elements() const noexcept { return elements_; }

    void clear() noexcept { elements_.clear(); }
    [[nodiscard]] std::size_t size() const noexcept { return elements_.size(); }

  private:
    std::vector<SceneElement> elements_;
};

} // namespace df

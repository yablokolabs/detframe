/// @file scene.cpp
#include "df/scene.hpp"

namespace df {

void Scene::add_line(int x0, int y0, int x1, int y1, Color c) noexcept {
    elements_.push_back({ElementType::Line, x0, y0, x1, y1, 0, 0, 0, c, nullptr, 0});
}

void Scene::add_rect(int x, int y, int w, int h, Color c) noexcept {
    elements_.push_back({ElementType::Rect, x, y, x + w, y + h, 0, 0, 0, c, nullptr, 0});
}

void Scene::add_filled_rect(int x, int y, int w, int h, Color c) noexcept {
    elements_.push_back({ElementType::FilledRect, x, y, x + w, y + h, 0, 0, 0, c, nullptr, 0});
}

void Scene::add_circle(int cx, int cy, int r, Color c) noexcept {
    elements_.push_back({ElementType::Circle, cx, cy, 0, 0, 0, 0, r, c, nullptr, 0});
}

void Scene::add_filled_circle(int cx, int cy, int r, Color c) noexcept {
    elements_.push_back({ElementType::FilledCircle, cx, cy, 0, 0, 0, 0, r, c, nullptr, 0});
}

void Scene::add_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color c) noexcept {
    elements_.push_back({ElementType::Triangle, x0, y0, x1, y1, x2, y2, 0, c, nullptr, 0});
}

void Scene::add_text(int x, int y, const char *text, Color c, int scale) noexcept {
    elements_.push_back({ElementType::Text, x, y, 0, 0, 0, 0, 0, c, text, scale});
}

} // namespace df

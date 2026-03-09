#pragma once
/// @file frame_timer.hpp
/// Fixed-timestep frame timer with budget monitoring.

#include <cstdint>
#include <time.h>

namespace df {

class FrameTimer {
  public:
    /// Create timer with target frame period in milliseconds.
    explicit FrameTimer(int target_ms) noexcept;

    /// Call at frame start. Returns frame number.
    std::uint64_t begin_frame() noexcept;

    /// Call at frame end. Waits until frame boundary. Returns render time in μs.
    std::int64_t end_frame() noexcept;

    /// Check if last frame exceeded budget.
    [[nodiscard]] bool overrun() const noexcept { return last_overrun_; }

    /// Last render time in microseconds.
    [[nodiscard]] std::int64_t last_render_us() const noexcept { return last_render_us_; }

    /// Worst render time in microseconds.
    [[nodiscard]] std::int64_t worst_render_us() const noexcept { return worst_render_us_; }

    /// Total frames rendered.
    [[nodiscard]] std::uint64_t frame_count() const noexcept { return frame_count_; }

    /// Total overruns.
    [[nodiscard]] std::uint64_t overrun_count() const noexcept { return overrun_count_; }

  private:
    std::int64_t target_ns_;
    std::int64_t frame_start_ns_;
    std::int64_t last_render_us_;
    std::int64_t worst_render_us_;
    std::uint64_t frame_count_;
    std::uint64_t overrun_count_;
    bool last_overrun_;
};

} // namespace df

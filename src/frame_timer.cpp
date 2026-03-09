/// @file frame_timer.cpp
#include "df/frame_timer.hpp"

namespace df {

static std::int64_t now_ns() noexcept {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<std::int64_t>(ts.tv_sec) * 1'000'000'000LL + ts.tv_nsec;
}

static void sleep_until(std::int64_t deadline_ns) noexcept {
    struct timespec ts;
    ts.tv_sec = deadline_ns / 1'000'000'000LL;
    ts.tv_nsec = deadline_ns % 1'000'000'000LL;
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, nullptr);
}

FrameTimer::FrameTimer(int target_ms) noexcept
    : target_ns_(static_cast<std::int64_t>(target_ms) * 1'000'000LL), frame_start_ns_(0),
      last_render_us_(0), worst_render_us_(0), frame_count_(0), overrun_count_(0),
      last_overrun_(false) {}

std::uint64_t FrameTimer::begin_frame() noexcept {
    frame_start_ns_ = now_ns();
    return frame_count_;
}

std::int64_t FrameTimer::end_frame() noexcept {
    auto render_ns = now_ns() - frame_start_ns_;
    last_render_us_ = render_ns / 1000;
    if (last_render_us_ > worst_render_us_) worst_render_us_ = last_render_us_;

    last_overrun_ = render_ns > target_ns_;
    if (last_overrun_) ++overrun_count_;

    // Wait for frame boundary
    auto deadline = frame_start_ns_ + target_ns_;
    if (now_ns() < deadline) sleep_until(deadline);

    ++frame_count_;
    return last_render_us_;
}

} // namespace df

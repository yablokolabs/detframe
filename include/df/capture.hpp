#pragma once
/// @file capture.hpp
/// Frame capture to PPM files (no external dependencies).
/// PPM is simple, portable, and easily verifiable.

#include "df/framebuffer.hpp"

namespace df {

/// Write framebuffer to a PPM file (P6 binary format).
/// Returns true on success.
bool write_ppm(const Framebuffer &fb, const char *path) noexcept;

/// Compare two PPM files byte-by-byte.
/// Returns true if identical.
bool compare_ppm(const char *path_a, const char *path_b) noexcept;

} // namespace df

# detframe

**Deterministic rendering pipeline for avionics display prototyping.**

Software rasterizer with fixed-point math, PFD widgets, and frame-level determinism verification. Every frame rendered with the same inputs produces bit-identical output — the foundational property for display system certification.

## What This Is

A rendering toolkit for prototyping avionics displays where determinism is non-negotiable:

- **Bit-exact rendering** — same inputs = identical pixels, every time, every platform
- **~437μs per PFD frame** (640×480) — 2.6% of 60fps budget
- **Zero floating-point in rasterization** — integer Bresenham, midpoint circle, scanline fill
- **Fixed-point math** — Q16.16 for trig (lookup table sin/cos, no libm in hot path)
- **Built-in PFD widgets** — attitude indicator, airspeed/altitude tapes, compass rose
- **PPM frame capture** — zero-dependency image output for regression testing
- **Frame diff tooling** — Python pixel comparator for CI visual regression

## What This Is NOT

- Not a GPU renderer (no OpenGL, Vulkan, or hardware acceleration)
- Not a certified display system (no DO-178C evidence package)
- Not pixel-compatible with ARINC 661 (inspired by, not compliant)
- Not production flight software

This is an engineering prototyping tool for exploring deterministic rendering before entering formal development.

## Quick Start

```bash
# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release -DDF_BUILD_BENCH=ON
cmake --build build -j$(nproc)

# Run PFD demo (30 frames)
./build/detframe --frames 30

# Capture frames to PPM files
./build/detframe --frames 10 --capture

# Compare captured frames
python3 tools/frame_diff.py frame_00000.ppm frame_00001.ppm

# Run determinism tests
cd build && ctest --output-on-failure

# Benchmark
./build/bench_render
```

## Demo Output

```
╔══════════════════════════════════════════════╗
║           detframe v0.1.0                    ║
║  Deterministic rendering pipeline            ║
╚══════════════════════════════════════════════╝

  Frame    0/30  render= 476μs  ok
  Frame   10/30  render= 464μs  ok
  Frame   20/30  render= 507μs  ok

══════════════════════════════════════════════
Results:
  Frames rendered: 30
  Worst render:    507μs
  Budget overruns: 0
══════════════════════════════════════════════
```

## PFD Widgets

### Attitude Indicator
Sky/ground split with pitch ladder, aircraft reference cross, and roll arc. Integer pixel offsets (2px per degree pitch).

### Airspeed / Altitude Tapes
Scrolling linear tape with major/minor tick marks, numeric labels, and current-value readout box. Configurable range, tick intervals, and scroll scale.

### Compass Rose
Horizontal heading indicator with cardinal labels (N/E/S/W), 10° minor ticks, 30° major ticks, and heading readout triangle.

### Text
Built-in 8×8 bitmap font (ASCII 32-126). No system fonts, no FreeType, no floating point. Scalable integer multiples.

## Architecture

```
┌──────────────────────────────────────────────┐
│  Scene / Widgets                             │
│  ┌──────────────┐ ┌──────────┐ ┌──────────┐ │
│  │ Attitude     │ │ Tape     │ │ Compass  │ │
│  └──────┬───────┘ └────┬─────┘ └────┬─────┘ │
│         ▼              ▼            ▼        │
│  ┌────────────────────────────────────────┐  │
│  │ Renderer (Bresenham, Midpoint, Scanln) │  │
│  │ All integer math — deterministic       │  │
│  └────────────────┬───────────────────────┘  │
│                   ▼                          │
│  ┌────────────────────────────────────────┐  │
│  │ Framebuffer (RGBA8888, row-major)      │  │
│  │ • set_pixel, blend_pixel, fill_rect    │  │
│  │ • equals(), diff_count() for verify    │  │
│  └────────────────┬───────────────────────┘  │
│                   ▼                          │
│  ┌──────────────┐ ┌──────────────────────┐   │
│  │ PPM Capture  │ │ Frame Timer          │   │
│  │ (P6 binary)  │ │ (CLOCK_MONOTONIC)    │   │
│  └──────────────┘ └──────────────────────┘   │
└──────────────────────────────────────────────┘
```

## Determinism Guarantees

The test suite verifies:

1. **Single frame** — rendering frame N twice produces bit-identical pixels
2. **Sequence** — rendering frames 0-49 twice produces 50 identical pairs
3. **Divergence** — different frame inputs produce different pixels (not trivially constant)

This is achieved by:
- No `float`/`double` in rasterization (integer Bresenham, midpoint, scanline)
- Lookup-table trigonometry (pre-computed at startup, Q16.16 fixed-point)
- Deterministic iteration order (top-to-bottom, left-to-right)
- No threading in render path
- No system-dependent font rendering

## Performance

Measured on 2.79 GHz x86_64 (Azure VM, single core):

| Metric | Value |
|--------|-------|
| Average render | 437 μs/frame |
| Max FPS | 2,289 fps |
| Budget usage | 2.6% of 16.67ms (60fps) |
| Resolution | 640×480 |
| Widgets | Attitude + 2 tapes + compass + text |

## Building

**Requirements:** Linux, CMake ≥ 3.25, GCC ≥ 13 or Clang ≥ 17

```bash
# Debug with tests
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DDF_BUILD_TESTS=ON
cmake --build build -j$(nproc)

# Release with benchmarks
cmake -B build -DCMAKE_BUILD_TYPE=Release -DDF_BUILD_BENCH=ON
cmake --build build -j$(nproc)
```

## Companion Repos

- **[partition-guard](https://github.com/yablokolabs/partition-guard)** — Time/space partitioning for mixed-criticality workloads
- **[wcet-probe](https://github.com/yablokolabs/wcet-probe)** — Measurement-based WCET analysis toolkit

Together: **isolate → validate → render**

## Limitations

1. **CPU-only** — no GPU acceleration (by design, for determinism)
2. **8×8 font only** — no anti-aliased text or variable-width fonts
3. **No alpha blending in widgets** — compositor supports it, widgets use solid fills
4. **PPM output only** — no PNG/BMP (add with stb_image_write if needed)
5. **No input handling** — pure rendering, no event loop or interaction

## References

- DO-178C § 6.4.3 — Display system verification
- ARINC 661 — Cockpit Display System (architectural inspiration only)
- Bresenham's line algorithm (1965) — integer rasterization
- Midpoint circle algorithm — integer circle drawing

## License

Apache-2.0

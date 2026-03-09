cmake_minimum_required(VERSION 3.25)
project(detframe
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Deterministic rendering pipeline for avionics display prototyping"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall -Wextra -Wpedantic -Werror
    -Wno-unused-parameter
)

# ── Library ──────────────────────────────────────────────────

add_library(df_core STATIC
    src/framebuffer.cpp
    src/renderer.cpp
    src/math.cpp
    src/frame_timer.cpp
    src/compositor.cpp
    src/scene.cpp
    src/capture.cpp
    src/widgets/attitude.cpp
    src/widgets/tape.cpp
    src/widgets/compass.cpp
    src/widgets/text.cpp
)

target_include_directories(df_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(df_core PUBLIC pthread rt)

# ── Main executable ──────────────────────────────────────────

add_executable(detframe src/main.cpp)
target_link_libraries(detframe PRIVATE df_core)

# ── Tests ────────────────────────────────────────────────────

option(DF_BUILD_TESTS "Build tests" ON)

if(DF_BUILD_TESTS)
    enable_testing()

    add_executable(test_math tests/test_math.cpp)
    target_link_libraries(test_math PRIVATE df_core)
    add_test(NAME test_math COMMAND test_math)

    add_executable(test_renderer tests/test_renderer.cpp)
    target_link_libraries(test_renderer PRIVATE df_core)
    add_test(NAME test_renderer COMMAND test_renderer)

    add_executable(test_determinism tests/test_determinism.cpp)
    target_link_libraries(test_determinism PRIVATE df_core)
    add_test(NAME test_determinism COMMAND test_determinism)
endif()

# ── Benchmarks ───────────────────────────────────────────────

option(DF_BUILD_BENCH "Build benchmarks" OFF)

if(DF_BUILD_BENCH)
    add_executable(bench_render bench/bench_render.cpp)
    target_link_libraries(bench_render PRIVATE df_core)
endif()

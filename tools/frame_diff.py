#!/usr/bin/env python3
"""
frame_diff.py — Compare two PPM frame captures for visual regression testing.

Usage:
    python3 frame_diff.py frame_a.ppm frame_b.ppm [--output diff.ppm]
"""

import argparse
import sys


def read_ppm(path: str) -> tuple[int, int, bytes]:
    """Read a P6 PPM file. Returns (width, height, rgb_data)."""
    with open(path, "rb") as f:
        magic = f.readline().strip()
        if magic != b"P6":
            print(f"Error: {path} is not P6 PPM", file=sys.stderr)
            sys.exit(1)

        # Skip comments
        line = f.readline()
        while line.startswith(b"#"):
            line = f.readline()

        w, h = map(int, line.split())
        maxval = int(f.readline().strip())
        if maxval != 255:
            print(f"Error: unsupported maxval {maxval}", file=sys.stderr)
            sys.exit(1)

        data = f.read(w * h * 3)
        return w, h, data


def write_ppm(path: str, w: int, h: int, data: bytes):
    """Write a P6 PPM file."""
    with open(path, "wb") as f:
        f.write(f"P6\n{w} {h}\n255\n".encode())
        f.write(data)


def diff_frames(a: bytes, b: bytes) -> tuple[int, bytes]:
    """Compare two RGB buffers. Returns (diff_count, diff_image)."""
    if len(a) != len(b):
        print("Error: frame sizes differ", file=sys.stderr)
        sys.exit(1)

    diff_count = 0
    diff_data = bytearray(len(a))

    for i in range(0, len(a), 3):
        if a[i:i+3] != b[i:i+3]:
            diff_count += 1
            # Highlight differences in red
            diff_data[i] = 255
            diff_data[i+1] = 0
            diff_data[i+2] = 0
        else:
            # Dim the matching pixels
            diff_data[i] = a[i] // 4
            diff_data[i+1] = a[i+1] // 4
            diff_data[i+2] = a[i+2] // 4

    return diff_count, bytes(diff_data)


def main():
    parser = argparse.ArgumentParser(description="Compare PPM frames")
    parser.add_argument("frame_a", help="First PPM frame")
    parser.add_argument("frame_b", help="Second PPM frame")
    parser.add_argument("--output", "-o", help="Output diff PPM", default=None)
    args = parser.parse_args()

    w1, h1, data_a = read_ppm(args.frame_a)
    w2, h2, data_b = read_ppm(args.frame_b)

    if w1 != w2 or h1 != h2:
        print(f"Error: dimensions differ ({w1}x{h1} vs {w2}x{h2})", file=sys.stderr)
        sys.exit(1)

    count, diff_img = diff_frames(data_a, data_b)
    total = w1 * h1

    if count == 0:
        print(f"✓ Frames are identical ({total:,} pixels)")
    else:
        pct = count / total * 100
        print(f"✗ {count:,} pixels differ ({pct:.2f}% of {total:,})")
        if args.output:
            write_ppm(args.output, w1, h1, diff_img)
            print(f"  Diff written to: {args.output}")

    sys.exit(0 if count == 0 else 1)


if __name__ == "__main__":
    main()

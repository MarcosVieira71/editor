#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_TYPE="${1:-Release}"

BUILD_ROOT="$SCRIPT_DIR/build"
BUILD_DIR="$BUILD_ROOT/$BUILD_TYPE"

mkdir -p "$BUILD_DIR"

conan install "$SCRIPT_DIR" \
  --output-folder="$BUILD_DIR" \
  --build=missing \
  -s build_type="$BUILD_TYPE"

cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$BUILD_DIR/conan_toolchain.cmake" \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

cmake --build "$BUILD_DIR"

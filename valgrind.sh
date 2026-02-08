#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN="$SCRIPT_DIR/build/Debug/editor"

valgrind \
  --tool=memcheck \
  --leak-check=full \
  --show-leak-kinds=definite \
  "$BIN"

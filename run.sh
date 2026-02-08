#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

BUILD_TYPE="${1:-Release}"
BIN="$SCRIPT_DIR/build/$BUILD_TYPE/editor"

if [[ ! -x "$BIN" ]]; then
  echo "Erro: binário não encontrado: $BIN"
  echo "Compile antes com: ./build.sh $BUILD_TYPE"
  exit 1
fi

exec "$BIN"

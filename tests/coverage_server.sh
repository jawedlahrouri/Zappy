#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="$ROOT/build-coverage"

cd "$ROOT"

cp "$ROOT/zappy_server" "$ROOT/zappy_server.bak" 2>/dev/null || true

cmake -S . -B "$BUILD" -DZAPPY_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug >/dev/null 2>&1
cmake --build "$BUILD" --target zappy_server -j >/dev/null

find "$BUILD" -name '*.gcda' -delete
python -m pytest "$ROOT/tests/test_zappy_com.py" -q

gcovr --root "$ROOT" --filter "$ROOT/server/" --txt --print-summary
if [ "${1:-}" = "--html" ]; then
    gcovr --root "$ROOT" --filter "$ROOT/server/" --html-details "$ROOT/coverage_server.html"
    echo "Rapport HTML : $ROOT/coverage_server.html"
fi

[ -f "$ROOT/zappy_server.bak" ] && mv "$ROOT/zappy_server.bak" "$ROOT/zappy_server"

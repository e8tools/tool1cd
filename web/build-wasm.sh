#!/usr/bin/env bash
set -euo pipefail

# Build tool1cd WASM API for browser usage.
# Output: web/parser.js + web/parser.wasm
#
# Prereqs:
# 1) Emscripten activated (em++ available)
# 2) Boost built for Emscripten (filesystem/system/regex)
#
# Example:
#BOOST_WASM_ROOT=$HOME/opt/boost-wasm ./web/build-wasm.sh

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="$ROOT_DIR/web"
SRC_DIR="$ROOT_DIR/src/tool1cd"

BOOST_WASM_ROOT="${BOOST_WASM_ROOT:-}"
if [[ -z "${BOOST_WASM_ROOT}" ]]; then
  echo "ERROR: Set BOOST_WASM_ROOT to your Boost-for-WASM prefix." >&2
  exit 1
fi

if ! command -v em++ >/dev/null 2>&1; then
  echo "ERROR: em++ not found. Activate emsdk first." >&2
  exit 1
fi

TOOL1CD_SOURCES=(
  main.cpp MessageRegistration.cpp Class_1CD.cpp
  Common.cpp ConfigStorage.cpp Parse_tree.cpp TempStream.cpp Base64.cpp UZLib.cpp Messenger.cpp
  V8Object.cpp Field.cpp Index.cpp Table.cpp TableFiles.cpp TableFileStream.cpp
  MemBlock.cpp CRC32.cpp Packdata.cpp PackDirectory.cpp FieldType.cpp DetailedException.cpp
  BinaryDecimalNumber.cpp save_depot_config.cpp save_part_depot_config.cpp
  SupplierConfig.cpp TableRecord.cpp BinaryGuid.cpp TableIterator.cpp SupplierConfigBuilder.cpp
  cfapi/V8File.cpp cfapi/V8Catalog.cpp cfapi/TV8FileStream.cpp cfapi/APIcfBase.cpp cfapi/V8Time.cpp
  SystemClasses/String.cpp SystemClasses/System.Classes.cpp SystemClasses/System.cpp
  SystemClasses/System.IOUtils.cpp SystemClasses/TFileStream.cpp SystemClasses/TMemoryStream.cpp
  SystemClasses/TStream.cpp SystemClasses/TStreamReader.cpp SystemClasses/TStreamWriter.cpp
  SystemClasses/System.SysUtils.cpp SystemClasses/GetTickCount.cpp
  wasm_api.cpp
)

ABS_SOURCES=()
for src in "${TOOL1CD_SOURCES[@]}"; do
  ABS_SOURCES+=("$SRC_DIR/$src")
done

mkdir -p "$OUT_DIR"

em++ \
  "${ABS_SOURCES[@]}" \
  -std=c++11 -O2 \
  -I"$SRC_DIR" \
  -I"$BOOST_WASM_ROOT/include" \
  -L"$BOOST_WASM_ROOT/lib" \
  -lboost_filesystem -lboost_system -lboost_regex \
  -sUSE_ZLIB=1 \
  -sFORCE_FILESYSTEM=1 \
  -sALLOW_MEMORY_GROWTH=1 \
  -sENVIRONMENT=web \
  -sMODULARIZE=0 \
  -sEXPORT_ES6=0 \
  -sNO_EXIT_RUNTIME=1 \
  -sEXPORTED_FUNCTIONS="['_malloc','_free','_onecd_open','_onecd_list_tables_json','_onecd_get_table_rows_json','_onecd_last_error','_onecd_close','_onecd_free_string']" \
  -sEXPORTED_RUNTIME_METHODS="['FS','cwrap','UTF8ToString']" \
  -o "$OUT_DIR/parser.js"

echo "Built: $OUT_DIR/parser.js and $OUT_DIR/parser.wasm"

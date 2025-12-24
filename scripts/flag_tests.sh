#!/bin/bash

# Runs a c++ file with all O flags

if [ $# -eq 0 ]; then 
    echo "No args provided"
    exit 1
fi

FILENAME="$1"
BASE_FOLDER="$2"
OUTPUT="${3:-test}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BIN_DIR="$PROJECT_ROOT/bin"
FILE_PATH="$BASE_FOLDER/$FILENAME" 
OUTPUT_PATH="$BIN_DIR/$OUTPUT"

FLAGS=("" "-O1" "-O2" "-O3" "-Ofast")

mkdir -p "$BIN_DIR"

for FLAG in "${FLAGS[@]}"; do
    g++ "$FILE_PATH" -o "$OUTPUT_PATH" -std=c++17 -ltbb $FLAG 2>&1

    if [ $? -ne 0 ]; then
        echo "Failed"
        exit 1
    fi

    echo -e "===================FLAG = ${FLAG:-None}========================="
    "$OUTPUT_PATH"
done


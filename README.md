# C++ Notes

A collection of performance benchmarks and general notes about C++, data structures and algorithms.

## Structure

- `cpp/` - C++ source files organized by topic
  - `memory/` - Memory layout, padding, alignment, and cache experiments
  - `performance/` - Performance benchmarks (integer types, accumulate/reduce, etc.)
  - `oop/` - Object-oriented programming experiments (vtables, inheritance, friends, constructors)
  - `misc/` - Miscellaneous experiments
- `docs/` - Documentation and notes
  - `notes.md` - Conceptual notes on C++ features
  - `experiments.md` - Experiments index and overview
- `scripts/` - Build and test scripts
- `bin/` - Compiled binaries

## Usage

To run experiments with different optimization flags:

```bash
./scripts/flag_tests.sh <filename> <folder>
# Example:
./scripts/flag_tests.sh pad_test.cpp memory
./scripts/flag_tests.sh type_test.cpp performance
``` 

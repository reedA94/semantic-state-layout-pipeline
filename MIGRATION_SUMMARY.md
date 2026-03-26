# Migration summary

## Modified files
- run_suite.py
- README.md
- src/core.py
- src/benchmarks.py
- src/layouts.py
- src/codegen.py

## New files
- configs/legacy.json
- configs/smoke.json
- configs/profile_shift.json
- configs/compiler_matrix.json
- configs/synthetic_sweep.json
- configs/full.json
- MODIFIED_FILES_FULL_CONTENTS.txt
- TESTING_NOTES.md

## Removed files
- none

## Config changes
- New config-driven execution model via JSON files.
- `python run_suite.py` keeps backward-compatible legacy behavior through `configs/legacy.json`.
- Stronger campaigns are exposed through dedicated configs rather than hard-coded suites.

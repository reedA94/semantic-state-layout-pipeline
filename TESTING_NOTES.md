# Testing notes

## Commands actually run in the container

### End-to-end smoke config
```bash
cd semantic_state_layout_pipeline
python -u run_suite.py --config configs/smoke.json
```

This completed successfully and produced the current summary files and plots.

### Targeted manual compile/run check
BM1 was also compiled and executed manually for B1, B4, and B5_DAG under GCC to verify the generated C scanners run correctly.

### Targeted matrix checks
Additional targeted internal runs exercised:
- GCC and Clang compilation paths
- synthetic benchmark generation and execution

## Current environment observations
- `clang` was available in the container.
- `perf` was not available, so the graceful-degradation path was exercised instead of actual hardware counter collection.

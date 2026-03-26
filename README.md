# Semantic state-layout synthesis benchmark suite

This artifact implements a reproducible benchmark pipeline for the manuscript on semantic state-layout synthesis for generated lexers / DFAs.

## Methods

- B1: arbitrary numbering + generic lookup dispatch
- B2: binary final/non-final threshold encoding
- B3: contiguous multi-class interval encoding
- B4: frequency-aware interval encoding
- B5_TREE: hierarchical comparison-tree encoding
- B5_DAG: hierarchical comparison-DAG encoding

## Benchmarks

The registry now includes BM1-BM7 plus synthetic families BM_SYN_S / BM_SYN_M / BM_SYN_L.

## Configuration entry point

```bash
python run_suite.py --config configs/full.json
```

## Current default config

`rollback_full`

## Output structure

- `corpora/<benchmark>/profiles/` and `corpora/<benchmark>/eval/`
- `raw_results/<benchmark>/<granularity>/<method>/<compiler>/<profile_scenario>/`
- `summaries/`
- `plots/`
- `environment.json`

## Environment snapshot

- OS: Linux-6.6.87.2-microsoft-standard-WSL2-x86_64-with-glibc2.39
- Python: 3.12.3 (main, Mar  3 2026, 12:15:18) [GCC 13.3.0]
- Compilers: clang_O2, clang_O3, clang_Os, gcc_O2, gcc_O3, gcc_Os
- perf available: True

# DPMH Assignment 2: Query Execution Models

**Data Processing on Modern Hardware** — TUM

A hands-on comparison of **classic and modern query execution models** in database systems: from Volcano-style iteration to vectorized and push-based execution.

## Topics Covered

- **Volcano model** (Iterator): row-at-a-time, pull-based
- **Vectorized execution**: batch processing for cache efficiency
- **Push-based model**: data-driven, JIT-friendly
- **Code generation**: compiling queries to native code
- Throughput comparison across models

## Structure

```
├── src/           # C++ source code
├── include/       # Headers
├── test/          # Unit tests
├── lib/           # External libraries
├── report.pdf     # Report with analysis
└── CMakeLists.txt # Build system
```

## Build & Run

```bash
mkdir build && cd build
cmake .. && make
./query_execution_models
```

## Key Results

Quantitative comparison showing the performance gap between iterator-at-a-time, vectorized, and code-generated execution across different query types and data sizes.

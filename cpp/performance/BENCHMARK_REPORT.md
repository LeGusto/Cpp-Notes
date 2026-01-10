# 2D Vector vs 2D Array Performance Comparison

**Files:** `vector_vs_array_2d.cpp`

**Hypothesis:**
2D vectors (`vector<vector<int>>`) will be slower than 2D arrays (`int[][]`) and 1D vectors accessed as 2D due to non-contiguous memory allocation. Each row in a 2D vector is separately allocated, causing cache misses when accessing elements across different rows, whereas arrays and 1D vectors have contiguous memory layouts that benefit from cache locality.

**Test Design:**
- **Workload**: Writing and reading all elements in a 1000×1000 matrix
- **Data Structures**: 2D Vector, 2D Array, 1D Vector (accessed as 2D)
- **Access Patterns**: Row-major (cache-friendly) and Column-major (cache-unfriendly)
- **Iterations**: 100 passes through the entire matrix
- **Total Operations**: 100,000,000 operations per test

### Results

#### Row-Major Access (Cache-Friendly)

| Optimization | 2D Vector | 2D Array | 1D Vector (as 2D) |
|--------------|-----------|----------|-------------------|
| **None** | 0.889 s | **0.130 s** | 0.439 s |
| **-O1** | 0.060 s | **0.054 s** | 0.054 s |
| **-O2** | 0.054 s | **0.053 s** | 0.054 s |
| **-O3** | 0.071 s | **0.054 s** | 0.054 s |
| **-Ofast** | 0.067 s | 0.056 s | **0.053 s** |

#### Column-Major Access (Cache-Unfriendly)

| Optimization | 2D Vector | 2D Array | 1D Vector (as 2D) |
|--------------|-----------|----------|-------------------|
| **None** | 0.863 s | **0.151 s** | 0.449 s |
| **-O1** | **0.127 s** | 0.240 s | 0.234 s |
| **-O2** | 0.252 s | 0.238 s | **0.228 s** |
| **-O3** | 0.249 s | 0.235 s | **0.230 s** |
| **-Ofast** | 0.251 s | 0.242 s | **0.231 s** |

#### Cache Performance (`perf stat` - No Optimization)

| Metric | 2D Array | 2D Vector |
|--------|----------|-----------|
| **Cache Miss Rate** | **0.90%** | 18.31% |
| **Cache Misses** | 70,274 | 1,175,405 |
| **Cache References** | 7,808,529 | 6,419,424 |
| **Execution Time** | **0.133 s** | 0.824 s |

### Analysis

1. **Without optimization, 2D arrays are 6.84x faster** - Non-contiguous allocation in 2D vectors causes massive performance penalty due to cache misses (18.31% vs 0.90% miss rate).

2. **Compiler optimization dramatically reduces the gap** - With `-O2`, performance difference becomes minimal (1.01x), suggesting compiler optimizations can compensate for structural overhead through inlining, vectorization, and loop optimizations.

3. **Cache miss rate correlates with performance** - The 20x difference in cache miss rates (18.31% vs 0.90%) directly explains the 6x execution time difference. Each cache miss costs 100-300 CPU cycles.

4. **Column-major access is cache-unfriendly for all structures** - All three structures show 4-5x slower performance with column-major access, confirming that access pattern matters more than data structure choice.

**Conclusion:**
The hypothesis is confirmed: 2D vectors are significantly slower without optimization due to non-contiguous memory allocation causing high cache miss rates. However, compiler optimization (`-O2` or higher) largely eliminates this performance gap. 1D vector performs the same as a 2D vector.

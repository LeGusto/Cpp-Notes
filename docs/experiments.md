# Performance Experiments

## Methodology

**Timing:** All experiments use `std::chrono::high_resolution_clock` for timing measurements.

**System Configuration:**
- **CPU:** Intel Core i7-12700H (12th Gen)
- **Architecture:** x86_64
- **Logical CPUs:** 20 (14 cores, 2 threads per core)
- **Max Frequency:** 4.7 GHz
- **Cache:** L1d 544 KiB, L1i 704 KiB, L2 11.5 MiB, L3 24 MiB
- **SIMD Support:** AVX, AVX2, SSE4.1, SSE4.2 (enables vectorization optimizations)
- **Compiler:** g++ with C++17 standard
- **Parallel Library:** TBB (Threading Building Blocks) for `std::execution::par` policies

## Integer Type Performance Comparison (CPU-Bound)

**Hypothesis:**
    Since my architecture is 64 bit, 64 bit types should perform the best and `int_fast*` types should map to 64 bits

**Test Design:**
- **Workload**: Heavy arithmetic operations (multiplication, bit shifts, XOR, modulo) performed in CPU registers
- **Data Size**: Small array (16 elements) 
- **Iterations**: 10^8 operations per test

**Test Operations:**
```cpp
result = result * 3 + values[i % ARRAY_SIZE];
result = (result << 2) ^ (result >> 1);
result = result % 1000 + 1;
result = result * 7 - values[(i + 1) % ARRAY_SIZE];
result = (result & 0xFF) | (result << 8);
```

### Results

| Type | Size | No Opt | -O1 | -O2 | -O3 | -Ofast |
|------|------|--------|-----|-----|-----|--------|
| **short** | 2 bytes | 1.811 s | 0.982 s | 0.883 s | 0.886 s | 0.896 s |
| **std::int16_t** | 2 bytes | 1.698 s | 0.918 s | 0.901 s | 0.856 s | 0.881 s |
| **int** | 4 bytes | 0.823 s | 0.918 s | 0.804 s | 0.779 s | 0.781 s |
| **std::int32_t** | 4 bytes | **0.788 s** | 0.782 s | 0.770 s | 0.787 s | 0.956 s |
| **long** | 8 bytes | 0.811 s | 0.825 s | **0.727 s** | 0.742 s | **0.719 s** |
| **long long** | 8 bytes | 0.795 s | **0.751 s** | 0.747 s | **0.738 s** | 0.874 s |
| **std::int64_t** | 8 bytes | 0.842 s | 0.760 s | 0.737 s | 0.750 s | 0.740 s |
| **std::int_fast32_t** | 8 bytes | 0.819 s | 0.771 s | 0.738 s | 0.753 s | 0.742 s |
| **std::int_fast64_t** | 8 bytes | 0.826 s | 0.839 s | 0.778 s | 0.753 s | 0.736 s |

### Analysis

1. **64-bit types perform best**

2. **`int_fast*` types correctly map to 64-bit sizes**

3. **16-bit types are slowest** 

**Conclusion:**
For CPU-intensive arithmetic operations on a 64-bit system, 64-bit types (`long`, `long long`, `int64_t`, or `int_fast64_t`) provide the best performance. The `int_fast*` types are a good general choice, since they automatically deduce the best size for CPU operations.

## Performance Comparison: accumulate() vs reduce()

**Hypothesis:**
    Parallel execution should provide the best performance, **reduce()** should always perform than **accumulate()**

**Test Design:**
- **Workload**: Summing all elements in a large vector 
- **Data Size**: Vector containing 10^7 random integers (values 1-100)
- **Iterations**: 100 summations per test 

**Test Operations:**
```cpp
// Accumulate (sequential, no policy)
std::accumulate(nums.begin(), nums.end(), 0, [](int sum, int b) {return sum + b;});

// Sequential Reduce (with execution policy)
std::reduce(std::execution::seq, nums.begin(), nums.end(), 0, [](int sum, int b) {return sum + b;});

// Unsequential Reduce (vectorization allowed, single-threaded)
std::reduce(std::execution::unseq, nums.begin(), nums.end(), 0, [](int sum, int b) {return sum + b;});

// Parallel Unsequential Reduce (multithreading + vectorization)
std::reduce(std::execution::par_unseq, nums.begin(), nums.end(), 0, [](int sum, int b) {return sum + b;});
```

### Results

| Optimization | Accumulate | Sequential Reduce | Unsequential Reduce | Parallel Unsequential Reduce |
|--------------|------------|-------------------|---------------------|------------------------------|
| **None** | 1.872 s | 2.570 s | 5.046 s | **0.873 s** |
| **-O1** | 0.393 s | 0.321 s | 0.446 s | **0.159 s** |
| **-O2** | 0.241 s | 0.316 s | 0.314 s | **0.119 s** |
| **-O3** | 0.232 s | 0.314 s | 0.271 s | **0.118 s** |
| **-Ofast** | 0.253 s | 0.313 s | 0.266 s | **0.117 s** |

### Analysis

1. **Parallel execution is fastest** - `par_unseq` consistently outperforms all other methods, likely due to multithreading and vectorization.

2. **Sequential reduce has policy overhead** - Without optimization, `std::execution::seq` is slower than `accumulate` due to execution policy mechanism overhead, but with optimization they perform similarly.

3. **Unsequential reduce is slowest without optimization** - `std::execution::unseq` performs worst at `-O0` because it optimizes the code for vectorization, however, the compiler doesn't actually use vectorization

**Conclusion:**
For memory-bound operations, `std::reduce` with `std::execution::par_unseq` provides the best performance by due to multithreading and vectorization. However, `std::accumulate` remains a good choice for simple sequential operations due to its lower overhead and guaranteed ordering.


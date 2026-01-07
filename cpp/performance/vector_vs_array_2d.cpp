#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstring>

const int ROWS = 1000;
const int COLS = 1000;
const int ITERS = 100;

// Test 2D vector (vector<vector<int>>)
double test_2d_vector_row_major() {
    std::vector<std::vector<int>> vec(ROWS, std::vector<int>(COLS, 0));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int sum = 0;
    for (int iter = 0; iter < ITERS; iter++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                vec[i][j] = i * COLS + j;
                sum += vec[i][j];
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1e6;
}

double test_2d_vector_col_major() {
    std::vector<std::vector<int>> vec(ROWS, std::vector<int>(COLS, 0));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int sum = 0;
    for (int iter = 0; iter < ITERS; iter++) {
        for (int j = 0; j < COLS; j++) {
            for (int i = 0; i < ROWS; i++) {
                vec[i][j] = i * COLS + j;
                sum += vec[i][j];
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1e6;
}

// Test 2D array (int[ROWS][COLS])
double test_2d_array_row_major() {
    static int arr[ROWS][COLS];
    memset(arr, 0, sizeof(arr));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int sum = 0;
    for (int iter = 0; iter < ITERS; iter++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                arr[i][j] = i * COLS + j;
                sum += arr[i][j];
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1e6;
}

double test_2d_array_col_major() {
    static int arr[ROWS][COLS];
    memset(arr, 0, sizeof(arr));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int sum = 0;
    for (int iter = 0; iter < ITERS; iter++) {
        for (int j = 0; j < COLS; j++) {
            for (int i = 0; i < ROWS; i++) {
                arr[i][j] = i * COLS + j;
                sum += arr[i][j];
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1e6;
}

// Test 1D vector accessed as 2D (for comparison)
double test_1d_vector_as_2d_row_major() {
    std::vector<int> vec(ROWS * COLS, 0);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int sum = 0;
    for (int iter = 0; iter < ITERS; iter++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                vec[i * COLS + j] = i * COLS + j;
                sum += vec[i * COLS + j];
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1e6;
}

double test_1d_vector_as_2d_col_major() {
    std::vector<int> vec(ROWS * COLS, 0);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    volatile int sum = 0;
    for (int iter = 0; iter < ITERS; iter++) {
        for (int j = 0; j < COLS; j++) {
            for (int i = 0; i < ROWS; i++) {
                vec[i * COLS + j] = i * COLS + j;
                sum += vec[i * COLS + j];
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1e6;
}

int main() {
    std::cout << "2D Vector vs 2D Array Benchmark\n";
    std::cout << "================================\n\n";
    std::cout << "Matrix size: " << ROWS << " x " << COLS << " = " 
              << (ROWS * COLS) << " elements\n";
    std::cout << "Iterations: " << ITERS << "\n";
    std::cout << "Total operations: " << (ROWS * COLS * ITERS) << "\n\n";
    
    std::cout << std::fixed << std::setprecision(6);
    
    std::cout << "Row-major access (cache-friendly):\n";
    std::cout << std::string(60, '-') << "\n";
    
    double time_2d_vec_row = test_2d_vector_row_major();
    double time_2d_arr_row = test_2d_array_row_major();
    double time_1d_vec_row = test_1d_vector_as_2d_row_major();
    
    std::cout << std::left << std::setw(35) << "2D Vector (vector<vector<int>>):" 
              << std::right << std::setw(12) << time_2d_vec_row << " s\n";
    std::cout << std::left << std::setw(35) << "2D Array (int[][])" 
              << std::right << std::setw(12) << time_2d_arr_row << " s\n";
    std::cout << std::left << std::setw(35) << "1D Vector (accessed as 2D):" 
              << std::right << std::setw(12) << time_1d_vec_row << " s\n";
    
    double speedup_arr = time_2d_vec_row / time_2d_arr_row;
    double speedup_1d = time_2d_vec_row / time_1d_vec_row;
    
    std::cout << "\nSpeedup (vs 2D vector):\n";
    std::cout << "  2D Array: " << std::setprecision(2) << speedup_arr << "x faster\n";
    std::cout << "  1D Vector: " << speedup_1d << "x faster\n";
    
    std::cout << "\n\nColumn-major access (cache-unfriendly):\n";
    std::cout << std::string(60, '-') << "\n";
    
    double time_2d_vec_col = test_2d_vector_col_major();
    double time_2d_arr_col = test_2d_array_col_major();
    double time_1d_vec_col = test_1d_vector_as_2d_col_major();
    
    std::cout << std::left << std::setw(35) << "2D Vector (vector<vector<int>>):" 
              << std::right << std::setprecision(6) << std::setw(12) << time_2d_vec_col << " s\n";
    std::cout << std::left << std::setw(35) << "2D Array (int[][])" 
              << std::right << std::setw(12) << time_2d_arr_col << " s\n";
    std::cout << std::left << std::setw(35) << "1D Vector (accessed as 2D):" 
              << std::right << std::setw(12) << time_1d_vec_col << " s\n";
    
    double speedup_arr_col = time_2d_vec_col / time_2d_arr_col;
    double speedup_1d_col = time_2d_vec_col / time_1d_vec_col;
    
    std::cout << "\nSpeedup (vs 2D vector):\n";
    std::cout << "  2D Array: " << std::setprecision(2) << speedup_arr_col << "x faster\n";
    std::cout << "  1D Vector: " << speedup_1d_col << "x faster\n";
    
    std::cout << "\n\nNotes:\n";
    std::cout << "- 2D arrays are contiguous in memory (better cache locality)\n";
    std::cout << "- 2D vectors store each row separately (may cause cache misses)\n";
    std::cout << "- 1D vector accessed as 2D combines benefits: contiguous + dynamic\n";
    std::cout << "- Column-major access is cache-unfriendly for all structures\n";
    
    return 0;
}


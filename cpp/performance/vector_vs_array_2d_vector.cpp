#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

const int ROWS = 1000;
const int COLS = 1000;
const int ITERS = 100;

int main() {
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
    
    std::cout << "2D Vector (vector<vector<int>>)\n";
    std::cout << "Matrix size: " << ROWS << " x " << COLS << "\n";
    std::cout << "Iterations: " << ITERS << "\n";
    std::cout << "Time: " << std::fixed << std::setprecision(6) 
              << duration.count() / 1e6 << " s\n";
    std::cout << "Sum: " << sum << "\n";
    
    return 0;
}



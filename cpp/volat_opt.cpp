#include <iostream>
#include <chrono>

int main() {
    // Test 1: WITH volatile
    {
        volatile int a[1] = {0};
        int b;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000000; i++) {
            b = a[0];
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "WITH volatile: " << time.count() << " ms\n";
        std::cout << "b = " << b << "\n";  // Use b so it's not optimized
    }
    
    // Test 2: WITHOUT volatile
    {
        int a[1] = {0};  // No volatile!
        int b;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000000; i++) {
            b = a[0];
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "WITHOUT volatile: " << time.count() << " ms\n";
        std::cout << "b = " << b << "\n";
    }
    
    return 0;
}
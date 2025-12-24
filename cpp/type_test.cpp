#include <iostream>
#include <cstdint>
#include <iomanip>
#include <chrono>
#include <vector>
#include <random>
#include <numeric>
#include <string>

const int ITERS = 1e8;  
const int ARRAY_SIZE = 16; 

template<typename IntType>
void test_type(const std::string& type_name) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<IntType> dist(1, 100);
    
    IntType values[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        values[i] = dist(rng);
    }
    
    IntType warmup = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        warmup += values[i];
    }
    (void)warmup;
    
    auto start = std::chrono::high_resolution_clock::now();
    IntType result = 1;
    for (int i = 0; i < ITERS; i++) {
        result = result * 3 + values[i % ARRAY_SIZE];
        result = (result << 2) ^ (result >> 1); 
        result = result % 1000 + 1;  
        result = result * 7 - values[(i + 1) % ARRAY_SIZE];
        result = (result & 0xFF) | (result << 8); 
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    volatile IntType volatile_result = result;
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double seconds = duration.count() / 1e6;
    
    std::cout << std::left << std::setw(25) << type_name 
              << std::fixed << std::setprecision(6) << seconds << " s"
              << " (size: " << sizeof(IntType) << " bytes)" << std::endl;
}

int main() {
    std::cout << "Testing integer type performance (CPU-bound):" << std::endl;
    std::cout << "Heavy arithmetic operations in CPU registers" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    test_type<int>("int");
    test_type<long>("long");
    test_type<long long>("long long");
    test_type<std::int32_t>("std::int32_t");
    test_type<std::int64_t>("std::int64_t");
    test_type<std::int_fast32_t>("std::int_fast32_t");
    test_type<std::int_fast64_t>("std::int_fast64_t");
    test_type<short>("short");
    test_type<std::int16_t>("std::int16_t");
    
    return 0;
}
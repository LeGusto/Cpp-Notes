#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>

const int ITERS = 100000000;
const int NUM_THREADS = 4;

// False sharing scenario: counters are on the same cache line
struct CountersBad {
    volatile int counter1;  // 4 bytes
    volatile int counter2;  // 4 bytes
    volatile int counter3;  // 4 bytes
    volatile int counter4;  // 4 bytes
    // Total: 16 bytes - fits in one cache line (typically 64 bytes)
    // But multiple threads writing to different counters causes false sharing
};

// Fixed: padding to separate cache lines (assuming 64-byte cache lines)
struct CountersGood {
    alignas(64) volatile int counter1;  // Align to cache line boundary
    
    alignas(64) volatile int counter2;
    
    alignas(64) volatile int counter3;
    
    alignas(64) volatile int counter4;
};

void increment_bad(volatile int* counter) {
    for (int i = 0; i < ITERS; i++) {
        (*counter)++;
    }
}

void benchmark_false_sharing() {
    CountersBad counters_bad;
    counters_bad.counter1 = 0;
    counters_bad.counter2 = 0;
    counters_bad.counter3 = 0;
    counters_bad.counter4 = 0;
    
    std::vector<std::thread> threads;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    threads.emplace_back(increment_bad, &counters_bad.counter1);
    threads.emplace_back(increment_bad, &counters_bad.counter2);
    threads.emplace_back(increment_bad, &counters_bad.counter3);
    threads.emplace_back(increment_bad, &counters_bad.counter4);
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "With false sharing:    " << std::setw(10) << duration.count() << " ms"
              << " (counters: " << counters_bad.counter1 << ", " << counters_bad.counter2 
              << ", " << counters_bad.counter3 << ", " << counters_bad.counter4 << ")" << std::endl;
}

void benchmark_no_false_sharing() {
    CountersGood counters_good;
    counters_good.counter1 = 0;
    counters_good.counter2 = 0;
    counters_good.counter3 = 0;
    counters_good.counter4 = 0;
    
    std::vector<std::thread> threads;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    threads.emplace_back(increment_bad, &counters_good.counter1);
    threads.emplace_back(increment_bad, &counters_good.counter2);
    threads.emplace_back(increment_bad, &counters_good.counter3);
    threads.emplace_back(increment_bad, &counters_good.counter4);
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Without false sharing: " << std::setw(10) << duration.count() << " ms"
              << " (counters: " << counters_good.counter1 << ", " << counters_good.counter2 
              << ", " << counters_good.counter3 << ", " << counters_good.counter4 << ")" << std::endl;
}

int main() {
    std::cout << "False Sharing Detection Test\n";
    std::cout << "============================\n\n";
    std::cout << "Cache line size: typically 64 bytes\n";
    std::cout << "Threads: " << NUM_THREADS << "\n";
    std::cout << "Increments per thread: " << ITERS << "\n\n";
    
    std::cout << "What is false sharing?\n";
    std::cout << "- Multiple threads WRITING to DIFFERENT variables on the SAME cache line\n";
    std::cout << "- Cache coherency protocol invalidates the entire cache line\n";
    std::cout << "- Causes cache line bouncing between CPU cores\n\n";
    
    std::cout << "Running benchmarks...\n\n";
    
    benchmark_false_sharing();
    benchmark_no_false_sharing();
    
    std::cout << "\nNote: False sharing causes the first run to be significantly slower.\n";
    std::cout << "Multiple threads writing to the same cache line cause excessive cache\n";
    std::cout << "invalidation and synchronization overhead between CPU cores.\n";
    
    return 0;
}


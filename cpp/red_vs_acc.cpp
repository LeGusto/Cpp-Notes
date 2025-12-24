#include <iostream>
#include <cstdint>
#include <iomanip>
#include <chrono>
#include <vector>
#include <random>
#include <execution>

const int ITEMS = 1e7;
const int ITERS = 1e2;

template<typename Duration>
void print_t(Duration start, Duration end, std::string_view msg) {
    std::string str = static_cast<std::string>(msg);
    while (str.size() < 50) str += " ";
    
    auto d = end - start;
    d = std::chrono::duration_cast<std::chrono::microseconds>(d);
    auto s = static_cast<double>(d.count()) / 1e9;
    std::cout<<str<<s<<" s\n";
}

template<typename ExecAlg>
void time_exec(ExecAlg alg, std::vector<int>& nums, std::string_view msg) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; i++)
        volatile int r = std::reduce(alg, nums.begin(), nums.end(), 0, [&](int sum, int b) {return sum + b;});
    auto end = std::chrono::high_resolution_clock::now();

    print_t(start, end, msg);
}


int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(1, 100);
    std::vector<int> nums(ITEMS, dist(rng));


    for (int i = 0; i < ITERS; i++)
        volatile int r = std::accumulate(nums.begin(), nums.end(), 0, [&](int sum, int b) {return sum + b;});


    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; i++)
        volatile int r = std::accumulate(nums.begin(), nums.end(), 0, [&](int sum, int b) {return sum + b;});
    auto end = std::chrono::high_resolution_clock::now();

    print_t(start, end, "Accumulate: ");

    time_exec(std::execution::seq, nums, "Sequential Reduce: ");
    time_exec(std::execution::unseq, nums, "Unsequential Reduce: ");
    time_exec(std::execution::par_unseq, nums, "Parallel Unsequential Reduce: ");
    return 0;


}
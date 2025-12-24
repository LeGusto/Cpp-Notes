#include <iostream>
#include <chrono>
#include <cstddef>

// #pragma pack(1)

// Struct WITH padding (bad alignment - char forces padding)
struct WithPadding {
    char a;      // 1 byte at offset 0
    // 3 bytes padding here
    int b;       // 4 bytes at offset 4 (aligned)
    char c;      // 1 byte at offset 8
    // 3 bytes padding at end
};

// Struct WITHOUT padding (good alignment - reordered)
struct WithoutPadding {
    int b;       // 4 bytes at offset 0 (aligned)
    char a;      // 1 byte at offset 4
    char c;      // 1 byte at offset 5
    // 2 bytes padding at end (for arrays)
};

int main() {
    const int iterations = 500000000; // Adjust to take ~1 second
    
    // Test 1: Struct WITH padding
    {
        WithPadding arr[1000];  // Array of structs with padding
        
        // Initialize
        for (int i = 0; i < 1000; i++) {
            arr[i].a = 'X';
            arr[i].b = i;
            arr[i].c = 'Y';
        }
        
        volatile int sum = 0;  // volatile to prevent optimization
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Access members - iterating through array
        for (int i = 0; i < iterations; i++) {
            int idx = i % 1000;
            sum += arr[idx].b;  // Access int member (aligned)
            sum += arr[idx].a;  // Access char member
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "WITH Padding:\n";
        std::cout << "  Size: " << sizeof(WithPadding) << " bytes\n";
        std::cout << "  Time: " << duration.count() << " ms\n";
        std::cout << "  Sum: " << sum << "\n\n";
    }
    
    // Test 2: Struct WITHOUT padding (better layout)
    {
        WithoutPadding arr[1000];  // Array of structs without padding
        
        // Initialize
        for (int i = 0; i < 1000; i++) {
            arr[i].b = i;
            arr[i].a = 'X';
            arr[i].c = 'Y';
        }
        
        volatile int sum = 0;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Same access pattern
        for (int i = 0; i < iterations; i++) {
            int idx = i % 1000;
            sum += arr[idx].b;  // Access int member (aligned)
            sum += arr[idx].a;  // Access char member
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "WITHOUT Padding (optimized layout):\n";
        std::cout << "  Size: " << sizeof(WithoutPadding) << " bytes\n";
        std::cout << "  Time: " << duration.count() << " ms\n";
        std::cout << "  Sum: " << sum << "\n";
    }
    
    // Show padding details
    std::cout << "\nPadding Analysis:\n";
    std::cout << "WithPadding offsets:\n";
    std::cout << "  a: " << offsetof(WithPadding, a) << "\n";
    std::cout << "  b: " << offsetof(WithPadding, b) << "\n";
    std::cout << "  c: " << offsetof(WithPadding, c) << "\n";
    
    std::cout << "\nWithoutPadding offsets:\n";
    std::cout << "  b: " << offsetof(WithoutPadding, b) << "\n";
    std::cout << "  a: " << offsetof(WithoutPadding, a) << "\n";
    std::cout << "  c: " << offsetof(WithoutPadding, c) << "\n";
    
    return 0;
}
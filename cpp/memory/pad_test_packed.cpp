#include <iostream>
#include <chrono>
#include <cstddef>

#pragma pack(1)

const int ITERS = 500000000;

struct WithPadding32 {
    char a_c = 'a';    
    int a_i = 1;   
    char b_c = 'b';  
    int b_i = 2;   
    char c_c = 'c';  
    int c_i = 3;   
    char d_c = 'd';
    int d_i = 4;  
};


struct WithoutPadding20 {
    char a_c = 'a';
    char b_c = 'b';
    char c_c = 'c';
    char d_c = 'd';
    int a_i = 1;
    int b_i = 2;
    int c_i = 3;
    int d_i = 4;
};

struct WithoutPadding16 {
    char a_c = 'a';
    char b_c = 'b';
    char c_c = 'c';
    char d_c = 'd';
    int a_i = 1;
    int b_i = 2;
    int c_i = 3;
};

template<typename Base>
void test(std::string_view s) {
    
    Base arr[1000] {};

    volatile int sum = 0;  
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < ITERS; i++) {
        int idx = i % 1000;
        sum += arr[idx].b_i; 
        sum += arr[idx].a_i;  
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
 
    std::string ans = static_cast<std::string>(s) + "Size: " + std::to_string(sizeof(Base));
    while (ans.size() < 40) ans += " ";
    ans += std::to_string(static_cast<double>(duration.count()) / 1e6) + " s\n";
    std::cout<<ans;
}

int main() {
    
    test<WithoutPadding20>("Aligned ");
    test<WithPadding32>("Packed Misaligned ");
    test<WithoutPadding16>("Aligned ");
       
    return 0;
}
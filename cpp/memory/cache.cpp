#include <iostream>
#include <chrono>

struct s {
    int a = 1;
    int get_a() volatile {std::cout<<"used\n"; return a;}
};

int main() {


    {
        volatile s a;
        int b = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 3; i++) {
            b += a.get_a();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "WITH volatile: " << time.count() << " ms\n";
        std::cout << "b = " << b << "\n"; 
    }
    
    {
        volatile s a;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 3; i++) {
            b += a.get_a();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "WITHOUT volatile: " << time.count() << " ms\n";
        std::cout << "b = " << b << "\n";
    }
    
    return 0;
}
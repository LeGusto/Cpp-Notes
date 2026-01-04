#include <iostream>
#include <iomanip>

class Base {
public:
    int b;
    int a;
    int c;
    
    Base(int x, int y) : b(x), a(b + y), c(a + b) {
        std::cout << "a = " << a << ", b = " << b << ", c = " << c << "\n";
    }
};

int main() {
    Base obj(1, 2);

    return 0;
}

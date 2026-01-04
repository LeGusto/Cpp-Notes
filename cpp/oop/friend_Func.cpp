#include <iostream>
#include <iomanip>

class Base {
private:
    int a = 5;
public:
    int b = 1;
    virtual void bruh() {return;}
    void func1(Base& b) {std::cout<<b.a<<"\n";}
    void func2() {std::cout<<a<<"\n";}
    void test() {
        std::cout<<this<<" "<<&a<<" "<<&b<<"\n";
    }
    int c = 1;
};

class steal {
    public:
        int a = -1;
        int b = 1;
        virtual void bruh() {return;}
};

int main() {
    Base a;
    Base* ptr = &a;
    steal* lol = reinterpret_cast<steal*>(ptr);

    a.test();
    std::cout<<&lol->a<<" "<<lol->a<<"\n";
}
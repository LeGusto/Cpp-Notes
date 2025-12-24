#include <iostream>
#include <cstdint>

class Base {
public:
    virtual void func1() { std::cout << "Base::func1\n"; }
    virtual void func2() { std::cout << "Base::func2\n"; }
    void func3() { std::cout << "Base::func3\n"; }
};

int main() {
    Base obj;

    void* vtable_ptr = &obj;
    void** test = (void**)&obj;
    // uintptr_t vtable_addr = *reinterpret_cast<uintptr_t*>(&obj);
    // void** vtable = reinterpret_cast<void**>(vtable_addr);
    
    // std::cout << "Vtable address: " << std::hex << vtable_addr << std::dec << "\n";
    // std::cout << "Function 1 address: " << std::hex << vtable[0] << std::dec << "\n";
    // std::cout << "Function 2 address: " << std::hex << vtable[1] << std::dec << "\n";   
    std::cout<<&obj<<": "<<vtable_ptr<<" "<<vtable_ptr<<"\n";
    std::cout<<&obj<<": "<<test<<" "<<*test<<"\n";
    std::cout<<&Base::func3<<"\n";

    
    
    return 0;
}
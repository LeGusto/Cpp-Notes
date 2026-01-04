#include <iostream>
#include <iomanip>

class Base {
public:
    virtual void func1() { std::cout << "Base::func1\n"; }
    virtual void func2() { std::cout << "Base::func2\n"; }
    void func3() { std::cout << "Base::func3\n"; }
};

class Derived : public Base {
    int a = 1;
    public:
        void func1() override {
            std::cout<<"Derived::func1";
        }
        virtual void func2() { std::cout<<"Derived::func2";}
};

class Empty {
    int a() {return 1;};
    int  b() {return 2;};
};


int main() {
    Empty emp;
    std::cout<<sizeof(emp)<<"\n";
    Base obj;
    Derived der;
    Base* weird = new Derived();
    Base weird2 = Derived();
    weird->func1();
    weird2.func1();
    std::cout<<"\n\n";

    std::cout<<typeid(obj).name()<<"\n";
    std::cout<<typeid(der).name()<<"\n";
    

    void** base_vtable = *(void***)&obj;
    void** derived_vtable = *(void***)&der;

    std::cout<<base_vtable<<" "<<derived_vtable<<"\n";

    std::cout<<sizeof(base_vtable)<<" "<<sizeof(derived_vtable)<<"\n";

    std::cout<<sizeof(obj)<<" "<<sizeof(der)<<"\n";
    

    // return 0;
    obj.func1(); der.func1();



    // std::cout<<arr[1]<<"\n";
}
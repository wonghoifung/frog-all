#include <iostream>

using namespace std;

class Base {
public:
    virtual void mf1() = 0;
    virtual void mf1(int);
};

void Base::mf1() {
    cout << "Base::mf1" << endl;
}

void Base::mf1(int) {
    cout << "Base::mf1(int)" << endl;
}

class Derived: private Base {
public:
    virtual void mf1() {
        Base::mf1();
    }
};

int main() {
    Derived d;
    //int x;
    d.mf1();
    //d.mf1(x);
}


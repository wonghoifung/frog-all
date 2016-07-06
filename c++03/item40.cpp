#include <iostream>

using namespace std;

class base {
  void* i;
public:
  virtual ~base() {}
};

class derived1: public base {
  void* i1;
};

class derived2: public base {
  void* i2;
};

class derived3: public derived1, public derived2 {
  void* i3;
};

class derived4: virtual public base {
  void* i4;
};

class derived5: virtual public base {
  void* i5;
};

class derived6: public derived4, public derived5 {
  void* i6;
};

int main() {
  cout << "   void* size: " << sizeof(void*) << endl;
  cout << "    base size: " << sizeof(base) << endl;
  cout << "derived3 size: " << sizeof(derived3) << endl; 
  cout << "derived6 size: " << sizeof(derived6) << endl; 
  cout << "derived1 size: " << sizeof(derived1) << endl;
  cout << "derived4 size: " << sizeof(derived4) << endl;
}


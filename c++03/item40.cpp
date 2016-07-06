#include <iostream>

using namespace std;

class base {
  int i;
public:
  virtual ~base() {}
};

class derived1: public base {
  int i1;
};

class derived2: public base {
  int i2;
};

class derived3: public derived1, public derived2 {
  int i3;
};

class derived4: virtual public base {
  int i4;
};

class derived5: virtual public base {
  int i5;
};

class derived6: public derived4, public derived5 {
  int i6;
};

int main() {
  cout << sizeof(void*) << endl;
  cout << sizeof(int) << endl; // 4
  cout << sizeof(derived3) << endl; // 40
  cout << sizeof(derived6) << endl; // 48
  cout << sizeof(derived1) << endl;
  cout << sizeof(derived4) << endl;
}


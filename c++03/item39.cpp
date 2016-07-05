#include <iostream>

using namespace std;

class Empty {};

class HoldsAnInt {
private:
  int x;
  Empty e;
};

class HoldsAnInt2: private Empty {
private:
  int x;
};

int main() {
  cout << sizeof(int) << endl;
  cout << sizeof(HoldsAnInt) << endl;
  cout << sizeof(HoldsAnInt2) << endl;
}


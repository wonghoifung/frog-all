#include <iostream>

using namespace std;

#if 0
template <typename T, size_t n>
class SquareMatrix {
public:
  T operate() {
    T res = 0;
    for (int i=0; i<n; ++i) {
      for (int j=1; j<n; ++j) {
        res += i;
        res *= j;
      }
    }
    return res;
  } 
};
#else
template <typename T>
class SquareMatrixBase {
protected:
  T operate(size_t n) {
    T res = 0;
    for (int i=0; i<n; ++i) {
      for (int j=1; j<n; ++j) {
        res += i;
        res *= j;
      }
    }
    return res;
  }
};
template <typename T, size_t n>
class SquareMatrix: private SquareMatrixBase<T> {
private:
  using SquareMatrixBase<T>::operate;
public:
  T operate() {this->operate(n);}
};
#endif

int main() {
  SquareMatrix<int, 5> s1; cout << s1.operate() << endl;
  {SquareMatrix<int, 1> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 2> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 3> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 4> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 15> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 25> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 35> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 45> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 55> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 65> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 75> s2; cout << s2.operate() << endl;}
  {SquareMatrix<int, 85> s2; cout << s2.operate() << endl;}
}


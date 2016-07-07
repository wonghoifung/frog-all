#include <iostream>

using namespace std;
#if 1
template <typename T>
class Rational {
  T numerator_;
  T denominator_;
public:
  Rational(const T& numerator = 0, const T& denominator = 1):
    numerator_(numerator),denominator_(denominator) {}
  const T numerator() const {return numerator_;}
  const T denominator() const {return denominator_;}
  
  friend const Rational operator*(const Rational& lhs, const Rational& rhs) {
  //friend const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
    return Rational<T>(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
  }
};
/*
template <typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
  return Rational<T>(lhs.numerator() * rhs.numerator(),
                     lhs.denominator() * rhs.denominator());
}
*/
#else
class Rational {
  int numerator_;
  int denominator_;
public:
  Rational(const int& numerator = 0, const int& denominator = 1):
    numerator_(numerator),denominator_(denominator) {}
  const int numerator() const {return numerator_;}
  const int denominator() const {return denominator_;}
};

const Rational operator*(const Rational& lhs, const Rational& rhs) {
  return Rational(lhs.numerator() * rhs.numerator(),
                  lhs.denominator() * rhs.denominator());
}
#endif

int main() {
  //Rational oneFourth(1,4);
  //Rational result;
  Rational<int> oneFourth(1,4);
  Rational<int> result;
  result = oneFourth * 2;
  result = 2 * oneFourth;
}


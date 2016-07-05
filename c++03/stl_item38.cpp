#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class BPFC;

template <typename T>
class BPFCImpl {
  int i;
  double d;
  string s;
  virtual void operator()(const T& val) const = 0;
  friend class BPFC<T>;  
public:
  virtual ~BPFCImpl() {} 
};

template <typename T>
class P: public BPFCImpl<T> {
  virtual void operator()(const T& val) const {
    cout << "P::operator() " << val << endl;
  }
};

template <typename T>
class BPFC: public unary_function<T, void> {
  BPFCImpl<T>* pImpl;
public:
  BPFC(BPFCImpl<T>* pi): pImpl(pi) {}
  void operator()(const T& val) const {
    pImpl->operator()(val);
  }
};

int main() {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  P<int> p;  
  BPFC<int> f(&p);
  for_each(v.begin(), v.end(), f);
}


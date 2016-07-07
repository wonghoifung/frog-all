#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <iterator>
#include <stdlib.h>
#include<sys/time.h>

using namespace std;

#define COUNT 100000

class Timekeeper {
  Timekeeper(const Timekeeper&);
  Timekeeper& operator=(const Timekeeper&);
  struct timeval start;
  string tag;
public:
  Timekeeper(const char* s):tag(s) {
    gettimeofday(&start,NULL);
  }
  ~Timekeeper() {
    struct timeval end;
    gettimeofday(&end,NULL);
    unsigned long diff = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    cout << tag << " time cost: " << diff << "usec" << endl;
  }
};

inline bool doubleGreater(double d1, double d2) { return d1 > d2; }

struct StringSize: public unary_function<string, string::size_type> {
  string::size_type operator()(const string& s) const {
    return s.size();
  }
};

template <typename T>
T average(T val) {
  return val / 2;
}

template <typename FPType>
FPType average(FPType val1, FPType val2) {
  return (val1 + val2) / 2;
}

template <typename FPType>
struct Average: public binary_function<FPType, FPType, FPType> {
  FPType operator()(FPType val1, FPType val2) const {
    return average(val1, val2);
  }
};

template <typename InputIter1, typename InputIter2>
void writeAverages(InputIter1 begin1, InputIter1 end1, InputIter2 begin2, ostream& s) {
  transform(begin1, end1, begin2,
    ostream_iterator<typename iterator_traits<InputIter1>::value_type>(s, "\n"),
    //average<typename iterator_traits<InputIter1>::value_type>
    Average<typename iterator_traits<InputIter1>::value_type>()
  );
}

int main() {
  vector<double> vv;
  for (int i=0; i<COUNT; ++i) {
    vv.push_back(rand()%COUNT);
  }

  vector<double> v = vv;
  { 
    Timekeeper tmp("functor");
    sort(v.begin(), v.end(), greater<double>());
  }

  v = vv;
  {
    Timekeeper tmp("function");
    sort(v.begin(), v.end(), doubleGreater);
  }

  //////////////

  set<string> s;
  s.insert("hello1");
  s.insert("hello22");
  s.insert("hello121");
  s.insert("hello242");
  s.insert("hello123123");
  transform(s.begin(), s.end(), ostream_iterator<string::size_type>(cout, "\n"), StringSize());
  //transform(s.begin(), s.end(), ostream_iterator<string::size_type>(cout, "\n"), mem_fun_ref(&string::size));

  /////////////

  vector<double> v1;
  v1.push_back(10);
  v1.push_back(20);
  v1.push_back(30);
  v1.push_back(40);
  v1.push_back(50);
  vector<double> v2;
  v2.push_back(10);
  v2.push_back(20);
  v2.push_back(30);
  v2.push_back(40);
  v2.push_back(50);
  writeAverages(v1.begin(), v1.end(), v2.begin(), cout);
}


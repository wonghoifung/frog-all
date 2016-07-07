#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include<sys/time.h>

using namespace std;

#define COUNT 10000000

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
}


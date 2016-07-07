#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

typedef vector<string> svector;
typedef svector::iterator sviterator;
typedef pair<sviterator, sviterator> svipair;

class Timestamp {
  int tsec;
public:
  Timestamp(int sec):tsec(sec) {
  }
  const int sec() const {return tsec;}
};
bool operator<(const Timestamp& lhs, const Timestamp& rhs) {
  return lhs.sec() < rhs.sec();
}

class Person {
  string pname;
public:
  Person(const char* n):pname(n) {}
  const string& name() const {return pname;}
};
struct PersonNameLess: public binary_function<Person, Person, bool> {
  bool operator()(const Person& lhs, const Person& rhs) const {
    return lhs.name() < rhs.name();
  }
};

int main() {
  svector vv;
  vv.push_back("abc6");
  vv.push_back("abc5");
  vv.push_back("abc4");
  vv.push_back("abc3");
  vv.push_back("abc2");
  vv.push_back("abc1");
  sort(vv.begin(), vv.end());

  svipair p = equal_range(vv.begin(), vv.end(), "abc3");
  if (p.first != p.second) {
    cout << "found abc3" << endl;
  } else {
    cout << "not found abc3" << endl;
  }

  cout << "abc3 count: " << distance(p.first, p.second) << endl;

  ///////////////////
  
  vector<Timestamp> vt;
  vt.push_back(Timestamp(45));
  vt.push_back(Timestamp(44));
  vt.push_back(Timestamp(43));
  vt.push_back(Timestamp(42));
  vt.push_back(Timestamp(41));
  sort(vt.begin(), vt.end());
  Timestamp ageLimit(43);
  vt.erase(vt.begin(), upper_bound(vt.begin(), vt.end(), ageLimit));
  //vt.erase(vt.begin(), lower_bound(vt.begin(), vt.end(), ageLimit));
  for (int i=0; i<vt.size(); ++i) {
    cout << vt[i].sec() << " ";
  }
  cout << endl;

  ////////////////////

  list<Person> lp;
  lp.push_back(Person("f"));
  lp.push_back(Person("e"));
  lp.push_back(Person("d"));
  lp.push_back(Person("c"));
  lp.push_back(Person("b"));
  lp.push_back(Person("a"));
  lp.sort(PersonNameLess());
  Person newPerson("cc");
  lp.insert(upper_bound(lp.begin(), lp.end(), newPerson, PersonNameLess()), newPerson);
  for (list<Person>::iterator it=lp.begin(); it!=lp.end(); ++it) {
    cout << it->name() << " ";
  }
  cout << endl;
}


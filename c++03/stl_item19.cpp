#include <string>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

int ciCharCompare(char c1, char c2) {
  int lc1 = tolower(static_cast<unsigned char>(c1));
  int lc2 = tolower(static_cast<unsigned char>(c2));
  if (lc1 < lc2) return -1;
  if (lc1 > lc2) return 1;
  return 0;
}

int ciStringCompareImpl(const string& s1, const string& s2) {
  typedef pair<string::const_iterator, string::const_iterator> PSCI;
  PSCI p = mismatch(s1.begin(), s1.end(), s2.begin(), not2(ptr_fun(ciCharCompare)));
  if (p.first == s1.end()) {
    if (p.second == s2.end()) return 0;
    return -1;
  }
  return ciCharCompare(*p.first, *p.second);
}

int ciStringCompare(const string& s1, const string& s2) {
  if (s1.size() <= s2.size()) return ciStringCompareImpl(s1, s2);
  return -ciStringCompareImpl(s2, s1);
}

struct CiStringCompare: public binary_function<string, string, bool> {
  bool operator()(const string& lhs, const string& rhs) const {
    return ciStringCompare(lhs, rhs);
  }
};

int main() {
  set<string, CiStringCompare> ciss;
  ciss.insert("Persephone");
  ciss.insert("persephone");
  cout << ciss.size() << endl;
  cout << *(ciss.begin()) << endl;
  cout << boolalpha << (ciss.find("persephone") != ciss.end()) << endl;
  cout << boolalpha << (find(ciss.begin(), ciss.end(), "persephone") != ciss.end()) << endl;
}


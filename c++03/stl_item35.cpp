#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>

using namespace std;

// 1

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

// 2

bool ciCharLess(char c1, char c2) {
  return tolower(static_cast<unsigned char>(c1)) < tolower(static_cast<unsigned char>(c2));
}

bool ciStringCompare2(const string& s1, const string& s2) {
  return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), ciCharLess);
}

int main() {
  string s1 = "HELLO";
  string s2 = "hello";
  cout << ciStringCompare(s1, s2) << endl;
  cout << boolalpha << ciStringCompare2(s1, s2) << endl;
}


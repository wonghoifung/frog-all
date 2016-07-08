#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// get rid of all the elements in the vector whose value is less than x
// except that elements preceding the last occurrence of a value at least as big as y
int main() {
  vector<int> v;
  v.push_back(5);
  v.push_back(4);
  v.push_back(21);
  v.push_back(10);
  v.push_back(31);
  v.push_back(8);
  v.push_back(20);
  v.push_back(1);
  v.push_back(9);
  v.push_back(7);
  int x=10, y=30;
  // result should be: 5 4 21 10 31 20
  v.erase(
    remove_if(
      find_if(v.rbegin(), v.rend(), bind2nd(greater_equal<int>(), y)).base(),
      v.end(),
      bind2nd(less<int>(), x)
    ),
    v.end()
  );
  for (int i=0; i<v.size(); ++i) {
    cout << v[i] << " ";
  }
  cout << endl;
}


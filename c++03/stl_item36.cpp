#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

template <typename InputIterator, typename OutputIterator, typename Predicate>
OutputIterator copy_if(InputIterator begin, InputIterator end, OutputIterator destBegin, Predicate p) {
  while (begin != end) {
    if (p(*begin)) *destBegin++ = *begin;
    ++begin;
  }
  return destBegin;
}

bool isOdd(int i) { return i % 2; }

void printIntVector(const vector<int>& v) {
    vector<int>::const_iterator cit = v.begin();
    for (; cit != v.end(); ++cit) {
        cout << *cit << " ";
    }
    cout << endl;
}

int main() {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(6);
  vector<int> outv;
  copy_if(v.begin(), v.end(), back_inserter(outv), isOdd);
  printIntVector(outv);
}


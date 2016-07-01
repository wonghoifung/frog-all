#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void printIntVector(const vector<int>& v) {
    vector<int>::const_iterator cit = v.begin();
    for (; cit != v.end(); ++cit) {
        cout << *cit << " ";
    }
    cout << endl;
}

int main() {
    vector<int> v;
    v.reserve(10);
    for (int i=1; i<=10; ++i) v.push_back(i);
    cout << v.size() << endl;
    v[3]=v[5]=v[9]=99;
    v.erase(remove(v.begin(), v.end(), 99), v.end());
    cout << v.size() << endl;
    printIntVector(v);
}


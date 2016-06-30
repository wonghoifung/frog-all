#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int transmogrify(int x) { return x; }

int main() {
    vector<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    values.push_back(4);
    values.push_back(5);
    values.push_back(6);
    vector<int> results; results.push_back(7);results.push_back(8);results.push_back(9);
    results.reserve(results.size() + values.size());
    //list<int> results;
    transform(values.begin(), values.end(), inserter(results, results.begin() + results.size() / 2), transmogrify);
    //transform(values.rbegin(), values.rend(), front_inserter(results), transmogrify);
    //transform(values.begin(), values.end(), front_inserter(results), transmogrify);
    vector<int>::iterator it = results.begin();
    //list<int>::iterator it = results.begin();
    for (; it != results.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}


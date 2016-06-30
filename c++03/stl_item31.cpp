#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

void printIntVector(const vector<int>& v) {
    vector<int>::const_iterator cit = v.begin();
    for (; cit != v.end(); ++cit) {
        cout << *cit << " ";
    }
    cout << endl;
}

void printIntVector(const vector<int>& v, vector<int>::iterator end) {
    vector<int>::const_iterator cit = v.begin();
    for (; cit != end; ++cit) {
        cout << *cit << " ";
    }
    cout << endl;
}

bool isOdd(int i) {
    return i & 1;
}

int main() {
    vector<int> arr;
    arr.push_back(4);
    arr.push_back(9);
    arr.push_back(1);
    arr.push_back(7);
    arr.push_back(2);
    arr.push_back(0);
    arr.push_back(8);
    arr.push_back(3);
    arr.push_back(6);
    arr.push_back(5);
    printIntVector(arr);
    //sort(arr.begin(), arr.end());
    //stable_sort(arr.begin(), arr.end());
    //nth_element(arr.begin(), arr.begin() + 5, arr.end());
    //partial_sort(arr.begin(), arr.begin() + 5, arr.end());
    //partial_sort(arr.begin(), arr.begin() + 5, arr.end(), greater<int>());
    //printIntVector(arr);
    
    /*
    vector<int>::iterator goalpos = arr.begin() + arr.size() / 2;
    cout << *goalpos << endl;
    nth_element(arr.begin(), goalpos, arr.end());
    printIntVector(arr);
    cout << *goalpos << endl;

    vector<int>::size_type goaloffset = 0.2 * arr.size();
    nth_element(arr.begin(), arr.begin() + goaloffset, arr.end());
    printIntVector(arr);
    cout << *goalpos << endl;
    cout << *(arr.begin() + goaloffset) << endl;
    */
    
    // partition stable_partition also support list
    vector<int>::iterator oddend = stable_partition(arr.begin(), arr.end(), isOdd);
    //vector<int>::iterator oddend = partition(arr.begin(), arr.end(), isOdd);
    printIntVector(arr, oddend);
}


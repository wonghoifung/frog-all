#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

using namespace std;

int main() {
    ifstream inputFile("stl_item27.cpp");
    // inputFile.unset(ios::skipws); // can't compile
    // string fileData((istream_iterator<char>(inputFile)), istream_iterator<char>());
    string fileData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    cout << "--->" << endl;
    cout << fileData << endl;
    cout << "--->" << endl;
}


#include <iostream>
#include <vector>

using namespace std;

vector<bool> features() { vector<bool> v{1,0,0,0}; return v; }

void print(bool b) { cout << boolalpha << b << endl; }

int main()
{  
  auto b = features()[0];

  print(b);

  return 0;
}


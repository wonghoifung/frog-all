#include <list>
#include <type_traits>
#include <typeinfo>
#include <iostream>

using namespace std;

int main()
{
	remove_const<const int>::type a;
	cout << typeid(a).name() << endl;

	//remove_const_t<const int> b; // c++14
	//cout << typeid(b).name() <<endl;

	return 0;
}

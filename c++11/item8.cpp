#include <iostream>
#include <memory>

using namespace std;

int f1(shared_ptr<double> p) 
{
	cout << "f1" << endl;
	return 0;
}

int f2(unique_ptr<double> p)
{
	cout << "f2" << endl;
	return 0;
}

int f3(double* p)
{
	cout << "f3" << endl;
	return 0;
}

template <typename FuncType, typename PtrType>
auto mycall(FuncType func, PtrType ptr) -> decltype(func(ptr))
{
	return func(ptr);
}

int main()
{
	f1(0);
	f2(NULL);
	f3(nullptr);

	mycall(f1,nullptr);
	mycall(f2,nullptr);
	mycall(f3,nullptr);

	return 0;
}

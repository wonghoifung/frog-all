#include <iostream>
#include <vector>

using namespace std;

void f(const std::vector<int>& v) 
{

}

template <typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}

template <typename... Ts>
void fwds(Ts&&... params)
{
	f(std::forward<Ts>(params)...);
}

int main()
{
	f({1,2,3});

	auto il = {1,2,3};
	fwd(il);

	return 0;
}
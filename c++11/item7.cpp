#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template<typename T, typename... Ts>
void print(Ts&&... params)
{
	//T obj(std::forward<Ts>(params)...);
	T obj{std::forward<Ts>(params)...};
	for (const auto& i:obj)
	{
		cout << i << " ";
	}
	cout << endl;
}

int main()
{
	print<std::vector<int>>(10,20);
	return 0;
}

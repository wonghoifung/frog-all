#include <iostream>
#include <vector>

using namespace std;

void f(const std::vector<int>& v) 
{

}

void f(int i)
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

class Widget {
public:
	static constexpr std::size_t MinVals = 100;
};
//constexpr std::size_t Widget::MinVals;

int main()
{
	f({1,2,3});
	auto il = {1,2,3};
	fwd(il);

	std::vector<int> v;
	v.reserve(Widget::MinVals);
	f(Widget::MinVals);
	//fwd(Widget::MinVals);

	return 0;
}
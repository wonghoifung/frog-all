#include <iostream>
#include <vector>
#include <memory>

using namespace std;

using FilterContainer = std::vector<std::function<bool(int)>>;

FilterContainer filters;

class Widget
{
public:
	Widget():divisor(2)
	{

	}
	void addFilter() //const
	{
		//filters.emplace_back([=](int val){return val%divisor==0;});

		auto dcpy = divisor;
		filters.emplace_back([dcpy](int val){return val%dcpy==0;});

		divisor=3;
	}

private:
	int divisor;
};

void doSomeWork()
{
	auto pw = unique_ptr<Widget>(new Widget);
	pw->addFilter();
}

void addFilter()
{
	//static auto divisor = 3;
	auto divisor = 3;
	filters.emplace_back([=](int val){return val%divisor==0;});
	++divisor;
}

int main()
{
	doSomeWork();
	if (filters.size()==1)
	{
		cout << "!" << endl;
		cout << boolalpha << filters[0](4) << endl;
	}

	addFilter();
	if (filters.size()==2)
	{
		cout << "!!" << endl;
		cout << boolalpha << filters[1](9) << endl;
	}
	return 0;
}
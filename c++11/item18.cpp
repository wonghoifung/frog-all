#include <iostream>
#include <memory>

using namespace std;

class Investment 
{
public:
	virtual ~Investment() {}
};

class Stock: public Investment {};
class Bond: public Investment{};
class RealEstate: public Investment{};

auto delInvmt = [](Investment* pi) 
{
	cout << "delete" << endl;
	delete pi;
};

template <typename... Ts>
unique_ptr<Investment, decltype(delInvmt)>
makeInvestment(Ts&&... params)
{
	unique_ptr<Investment, decltype(delInvmt)> p(nullptr, delInvmt);
	p.reset(new Stock(forward<Ts>(params)...));
	return p;
}

int main()
{
	auto p = makeInvestment();
	return 0;
}
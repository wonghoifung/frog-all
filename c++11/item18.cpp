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

class C
{
public: 
	C(){cout<<"C"<<endl;}
	~C(){cout<<"~C"<<endl;}
};

int main()
{
	auto p = makeInvestment();

	unique_ptr<C[]> pc(new C[2]);

	return 0;
}
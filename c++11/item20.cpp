#include <iostream>
#include <memory>

using namespace std;

class C
{
public: 
	C(){cout<<"C"<<endl;}
	~C(){cout<<"~C"<<endl;}
};

int main()
{
	shared_ptr<C> p(new C);

	weak_ptr<C> wp(p);

	if (wp.expired())
	{
		cout << "expired" << endl;
	}
	
	cout << "---" << endl;

	p = nullptr;

	if (wp.expired())
	{
		cout << "expired" << endl;
	}

	return 0;
}
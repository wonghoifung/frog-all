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
	{
		auto p = make_shared<C>();

		cout<<"------- create weak_ptr"<<endl;

		weak_ptr<C> wp = p;
		
		cout<<"------- "<<wp.expired()<<endl;

		p = nullptr;

		cout<<"------- reset weak_ptr "<<wp.expired()<<endl;

		wp.reset();

		cout<<"-------"<<endl;
	}

	return 0;
}
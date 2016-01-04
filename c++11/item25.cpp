#include <iostream>

using namespace std;

class C
{
public: 
	C(){cout<<"C"<<endl;}
	~C(){cout<<"~C"<<endl;}
	C(const C& c){cout<<"C&"<<endl;}
	C(C&& c){cout<<"C&&"<<endl;}	
	C& operator=(const C& c){cout<<"C&="<<endl;}
	C& operator=(C&& c){cout<<"C&&="<<endl;}
};

C get()
{
	C c;
	//return std::move(c);
	return c;//rvo
}

C get2(C&& c)
{
	//return c;
	return std::move(c);
}

template <typename T>
T get3(T&& t)
{
	return std::forward<T>(t);
}

C get4(const C& c)
{
	//return std::move(c);
	return c;
}

C get5(C c)
{
	//return std::move(c);
	return c;
}

int main()
{
	{
		C c = get();
	}
	cout<<"----------------------"<<endl;
	{
		C c2 = get2(get());
	}
	cout<<"----------------------"<<endl;
	{
		C c3 = get3(get());
	}
	cout<<"----------------------"<<endl;
	{
		C c;
		C c4 = get4(c);
	}
	cout<<"----------------------"<<endl;
	{
		C c5 = get5(get());
	}
	return 0;
}


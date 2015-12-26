#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class CC
{
	static int count;
	int id;
public:
    const int getid() const {return id;}
	CC() :id(++count) {cout<<"CC "<<id<<endl;}
	CC(const CC& cc) :id(++count) {cout<<"CC& copy "<<cc.id<<" to "<<id<<endl;}
	CC(CC&& cc) noexcept :id(++count) {cout<<"CC&& move "<<cc.id<<" to "<<id<<endl;}
	CC& operator=(CC&& cc) 
	{
		cout<<"CC=&& "<<id<<" move id:"<<cc.id<<endl;
		if (this != &cc)
		{
			swap(id,cc.id);
		}
		return *this;
	}
	CC& operator=(const CC& cc) 
	{
		cout<<"CC=& "<<id<<" copy id:"<<cc.id<<endl;
		if (this != &cc)
		{
			id=cc.id;
		}
		return *this;
	}
};
int CC::count = 0;

void print(std::vector<CC>& v)
{
	for (const auto& c : v)
	{
		cout << c.getid() << " ";
	}
	cout << endl;
}

int main() 
{
	std::vector<CC> v;
	CC c1,c2,c3,c4;
	cout<<"v cap: "<<v.capacity()<<endl;

	v.push_back(c1);
	cout<<"v cap: "<<v.capacity()<<endl;
	v.push_back(c2);
	cout<<"v cap: "<<v.capacity()<<endl;
	v.push_back(c3);
	cout<<"v cap: "<<v.capacity()<<endl;
	v.push_back(c4);
	cout<<"v cap: "<<v.capacity()<<endl;

	cout<<"--------------------"<<endl;

	CC c5,c6;
	std::vector<CC> v2;
	v2.push_back(c5);
	v2.push_back(c6);

	cout<<"--------------------"<<endl;
	
	cout<<"v: "; print(v);
	cout<<"v2: "; print(v2);
	swap(v,v2);
	cout<<"v: "; print(v);
	cout<<"v2: "; print(v2);

	cout<<"--------------------"<<endl;

	CC c7,c8;
	cout<<"c7 id: "<<c7.getid()<<endl;
	cout<<"c8 id: "<<c8.getid()<<endl;
	swap(c7,c8);
	cout<<"c7 id: "<<c7.getid()<<endl;
	cout<<"c8 id: "<<c8.getid()<<endl;

	cout<<"--------------------"<<endl;

	CC arr1[3];
	CC arr2[3];
	swap(arr1,arr2);

	return 0;
}

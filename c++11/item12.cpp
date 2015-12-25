#include <iostream>

using namespace std;

class DT
{
public:
	DT() {}
	DT(const DT& dt) {cout<<"copy ctor"<<endl;}
	DT(const DT&& dt) {cout<<"move ctor"<<endl;}
};

class Widget 
{
public:
	void doWork() &
	{
		cout << "doWork &" << endl;
	}
	void doWork() &&
	{
		cout << "doWork &&" << endl;
	}

	DT& data() & {return values;}
	DT&& data() && {return move(values);}
private:
	DT values;
};

Widget makeWidget()
{
	return Widget();
}

int main()
{
	Widget w;
	w.doWork();
	makeWidget().doWork();

	auto v1 = w.data();
	auto v2 = makeWidget().data();

	return 0;
}

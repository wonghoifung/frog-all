#include "item22.h"
#include <iostream>

using namespace std;

int main()
{
	Widget w;

	cout<<"----> declare sw1"<<endl;
	sWidget sw1;
	sw1.setName("abc");
	cout<<"sw1:";sw1.printName();

	cout<<"----> move construct sw2"<<endl;
	sWidget sw2(std::move(sw1));
	cout<<"sw1:";sw1.printName();
	cout<<"sw2:";sw2.printName(); 

	cout<<"----> move assign sw1"<<endl;
	sw1=std::move(sw2);
	cout<<"sw1:";sw1.printName();
	cout<<"sw2:";sw2.printName();

	return 0;
}

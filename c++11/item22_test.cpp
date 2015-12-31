#include "item22.h"

int main()
{
	Widget w;

	sWidget sw1;
	sw1.setName("abc");
	sw1.printName();

	sWidget sw2(std::move(sw1));
	sw1.printName();
	sw2.printName(); // TODO crash

	sw1=std::move(sw2);
	sw1.printName();
	sw2.printName();

	return 0;
}
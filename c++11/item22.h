#ifndef item22_header
#define item22_header

#include <memory>

class Widget {
public:
	Widget();
	~Widget();
	Widget(Widget&& rhs);
	Widget& operator=(Widget&& rhs);
	Widget(const Widget& rhs);
	Widget& operator=(const Widget& rhs);

private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};

class sWidget {
public:
	sWidget();
	void printName();
	void setName(const char* n);

private:
	struct Impl;
	std::shared_ptr<Impl> pImpl;
};

#endif

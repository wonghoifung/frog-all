#include "item22.h"
#include <string>
#include <vector>
#include <iostream>

struct Widget::Impl {
	std::string name;
	std::vector<double> data;
};

Widget::Widget():pImpl(new Impl())
{

}

Widget::~Widget() = default;
Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

Widget::Widget(const Widget& rhs) : pImpl(nullptr)
{
	if (rhs.pImpl) pImpl.reset(new Impl(*rhs.pImpl));
}

Widget& Widget::operator=(const Widget& rhs)
{
	if (!rhs.pImpl) pImpl.reset();
	else if (!pImpl) pImpl.reset(new Impl(*rhs.pImpl));
	else *pImpl = *rhs.pImpl;
	return *this;
}

////////////

struct sWidget::Impl {
	std::string name;
	std::vector<double> data;
};

sWidget::sWidget():pImpl(std::make_shared<Impl>())
{

}

void sWidget::printName()
{
	if (!pImpl) {std::cout<<"p null impl"<<std::endl;return;}
	std::cout<<pImpl->name<<std::endl;
}

void sWidget::setName(const char* n)
{
	if (!pImpl) {std::cout<<"s null impl"<<std::endl;return;}
	pImpl->name=n;
}

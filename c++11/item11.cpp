#include <iostream>

bool isLucky(int number)
{
	std::cout << "isLucky: " << number << std::endl;
	return true;
}
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;

template <typename T>
void process(T* p)
{
	std::cout << "process" << std::endl;
}
template <>
void process<void>(void*) = delete;
template <>
void process<char>(char*) = delete;

class Widget 
{
public:
	template <typename T>
	void process(T* p)
	{
		std::cout << "Widget::process" << std::endl;
	}
// private:
// 	template <>
// 	void process<char>(char*) = delete;
};
template <>
void Widget::process<char>(char*) = delete;

int main()
{
	isLucky(1);
	// isLucky('a');
	// isLucky(true);
	// isLucky(5.2);

	int i = 1;
	char c = 'a';
	process(&i);
	// process(&c);

	Widget w;
	w.process(&i);
	// w.process(&c);

	return 0;
}

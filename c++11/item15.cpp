#include <iostream>
#include <array>

constexpr int pow(int base, int exp) noexcept
{
	return (exp == 0 ? 1 : base * pow(base, exp - 1));
}

class Point
{
public:
	constexpr Point(double xv=0, double yv=0) noexcept :x(xv),y(yv) {}
	constexpr double getx() const noexcept {return x;}
	constexpr double gety() const noexcept {return y;}
	void setx(double xv) noexcept {x=xv;}
	void sety(double yv) noexcept {y=yv;}
private:
	double x,y;
};

constexpr Point midPoint(const Point& p1, const Point& p2) noexcept
{
	return {(p1.getx()+p2.getx())/2, (p1.gety()+p2.gety())/2};
}

int main()
{
	std::array<int, pow(3, 2)> arr;
	std::cout<<arr.size()<<std::endl;

	constexpr Point p1(8,4);
	constexpr Point p2(2,6);
	constexpr auto mid = midPoint(p1,p2);
	constexpr int arr2[static_cast<int>(mid.getx() * mid.getx() - mid.gety())] = {0};
	std::cout<<sizeof(arr2)<<std::endl; // 20 * 4
	return 0;
}


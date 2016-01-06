#include <iostream>
#include <vector>

using namespace std;

struct header {
	std::uint32_t version:4,
				  totalLength:16;
};

void f(std::size_t sz)
{

}

template <typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}

int main()
{
	header h;

	f(h.totalLength);

	std::uint16_t len = h.totalLength;
	fwd(len);

	return 0;
}
#include <vector>
#include <functional>

using namespace std;

int main()
{
	std::vector<double> v;

	auto func = bind([](const std::vector<double>& v){}, std::move(v));

	return 0;
}
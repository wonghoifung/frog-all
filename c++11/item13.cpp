#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef std::vector<int>::iterator IterT;
typedef std::vector<int>::const_iterator ConstIterT;

template <typename C>
auto cbegin(const C& container) -> decltype(begin(container))
{
	return begin(container);
}

template <typename C>
auto cend(const C& container) -> decltype(end(container))
{
	return end(container);
}

template <typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
	auto it = find(cbegin(container), cend(container), targetVal);
	container.insert(it, insertVal);
}

int main()
{
	std::vector<int> values{1,2,3,4,5,6};

	IterT i = find(values.begin(), values.end(), 3);
	values.insert(i, 1998);

	ConstIterT ci = find(values.cbegin(), values.cend(), 3);
	values.insert(ci, 2011);

	findAndInsert(values, 3, 2017);

	copy(values.cbegin(), values.cend(), ostream_iterator<int>(cout, " ")); cout << endl;

	int arr[]{1,2,3,4,5,6};
	//findAndInsert(arr, 3, 2017);
	copy(cbegin(arr), cend(arr), ostream_iterator<int>(cout, " ")); cout << endl;

	return 0;
} 



template <typename MapType, typename KeyArgType, typename ValueArgType>
typename MapType::iterator efficientAddOrUpdate(MapType& m, const KeyArgType& k, const ValueArgType& v)
{
	typename MapType::iterator lb = m.lower_bound(k);
	if (lb != m.end() && !(m.key_comp()(k, lb->first))) {
		lb->second = v;
		return lb;
	} 
	else {
		typedef typename MapType::value_type MVT;
		return m.insert(lb, MVT(k, v));
	}
}

#include <map>
#include <string>
#include <iostream>

void printMap(std::map<int,std::string>& m) 
{
	std::map<int,std::string>::iterator it = m.begin();
	for (; it!=m.end(); ++it)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	}
}

int main()
{
	std::map<int, std::string> m;
	efficientAddOrUpdate(m, 1, "abc");
	efficientAddOrUpdate(m, 2, "xyz");
	printMap(m);
	efficientAddOrUpdate(m, 1, "mno");
	printMap(m);
	return 0;
}

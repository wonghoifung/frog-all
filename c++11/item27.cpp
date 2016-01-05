#include <iostream>
#include <chrono>
#include <set>
#include <string>
#include <type_traits>
#include <time.h>

using namespace std;

std::multiset<std::string> names;

const std::string getCurrentSystemTime()
{
  auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm* ptm = localtime(&tt);
  char date[60] = {0};
  sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
    (int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday,
    (int)ptm->tm_hour,(int)ptm->tm_min,(int)ptm->tm_sec);
  return std::string(date);
}

template <typename T>
void logAndAddImpl(T&& name, std::false_type)
{
	auto now = getCurrentSystemTime();
	cout << now << " ft: " << name << endl;
}

void logAndAddImpl(int idx, std::true_type)
{
	auto now = getCurrentSystemTime();
	cout << now << " tt: " << idx << endl;
}

template <typename T>
void logAndAdd(T&& name)
{
	logAndAddImpl(
		std::forward<T>(name), 
		std::is_integral<typename std::remove_reference<T>::type>()
		);
}

int main()
{
	std::string petName("darla");
	logAndAdd(petName);
	logAndAdd(std::string("peee"));
	logAndAdd("bbbbb");
	logAndAdd(22);
	logAndAdd(22u);
	return 0;
}
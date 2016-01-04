#include <iostream>
#include <chrono>
#include <set>
#include <string>
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
void logAndAdd(T&& name)
{
	auto now = getCurrentSystemTime();
	cout << now << ": " << name << endl;
	names.emplace(std::forward<T>(name));
}

void logAndAdd(int idx) 
{
	auto now = getCurrentSystemTime();
	cout << now << ": " << idx << endl;
}

int main()
{
	std::string petName("darla");
	logAndAdd(petName);
	logAndAdd(std::string("peee"));
	logAndAdd("bbbbb");
	logAndAdd(22);
	//logAndAdd(22u);
	return 0;
}
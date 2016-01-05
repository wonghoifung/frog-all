#include <iostream>
#include <chrono>
#include <set>
#include <string>
#include <sstream>
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

std::string nameFromIdx(int idx) 
{
	std::stringstream ss;
	ss << "name_" << idx;
	return ss.str();
}

class Person
{
public:
	Person() {cout<<"Person() called"<<endl;}

	template <
		typename T,
		typename = typename enable_if< !is_base_of<Person, typename std::decay<T>::type>::value &&
									   !is_integral<typename remove_reference<T>::type>::value >::type
	>
	explicit Person(T&& n) :name(std::forward<T>(n)) 
	{
		static_assert(is_constructible<string,T>::value, "parameter n can't be used to construct a string"); 
		cout<<"Person(T&&) called"<<endl; 
	}

	explicit Person(int idx) :name(nameFromIdx(idx)) { cout<<"Person(int) called"<<endl; }

	Person(const Person& rhs) :name(rhs.name) { cout<<"Person(const Person&) called"<<endl; }

	Person(Person&& rhs) :name(std::move(rhs.name)) { cout<<"Person(Person&&) called"<<endl; }

private:
	std::string name;
};

class SpecialPerson : public Person
{
public:
	SpecialPerson():Person() {}
	SpecialPerson(const SpecialPerson& rhs):Person(rhs) {}
	SpecialPerson(SpecialPerson&& rhs):Person(std::move(rhs)) {}
};

int main()
{
	Person p("albert");
	Person p2(p);

	cout<<endl;

	SpecialPerson sp;
	SpecialPerson sp2(sp);

	cout<<endl;

	Person p3((short)3);

	return 0;
}

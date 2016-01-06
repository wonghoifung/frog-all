#include <iostream>
#include <vector>

using namespace std;

void f(int (*pf)(int))
{

}

template <typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}

int proval(int val) {return 0;}
int proval(int val, int pri) {return 0;}

template <typename T>
T www(T t) {return T();}

int main()
{
	f(proval);

	using PT = int (*)(int);
	PT p = proval;
	fwd(p);

	f(www);
	fwd(static_cast<PT>(www));

	return 0;
}
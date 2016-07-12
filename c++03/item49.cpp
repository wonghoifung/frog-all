#include <iostream>
#include <stdlib.h>

using namespace std;

class NewHandlerHolder {
public:
  explicit NewHandlerHolder(std::new_handler nh):handler(nh) {}
  ~NewHandlerHolder() {std::set_new_handler(handler);}
private:
  std::new_handler handler;
  NewHandlerHolder(const NewHandlerHolder&);
  NewHandlerHolder& operator=(const NewHandlerHolder&);
};

template <typename T>
class NewHandlerSupport {
public:
  static std::new_handler set_new_handler(std::new_handler p) throw();
  static void* operator new(std::size_t size) throw(std::bad_alloc);
private:
  static std::new_handler currentHandler;
};

template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
{
  std::new_handler oldHandler = currentHandler;
  currentHandler = p;
  return oldHandler;
}

template <typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) throw(std::bad_alloc)
{
  NewHandlerHolder h(std::set_new_handler(currentHandler));
  return ::operator new(size);
}

template <typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = 0;

class Widget: public NewHandlerSupport<Widget>
{
  char data[100000000000L];
};

void outOfMem() {
  std::cerr << "not enough memory" << std::endl;
  abort();
}

class Widget2 
{
  char data[100000000000L];
};

// CRTP (curiously recurring template pattern)
int main() {
  //Widget::set_new_handler(outOfMem);  
  //Widget* pw = new Widget;
  
  Widget2* pw2 = new Widget2;
  //Widget2* pw2 = new (std::nothrow) Widget2;
  if (pw2 == 0) cout << "cannot alloc memory" << endl;
}


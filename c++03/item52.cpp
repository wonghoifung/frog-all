#include <iostream>
#include <exception>

using namespace std;

class StandardNewDeleteForms {
public:
  // normal new/delete
  static void* operator new(std::size_t size) throw(std::bad_alloc)
  { return ::operator new(size); }
  static void operator delete(void* pMemory) throw()
  { ::operator delete(pMemory); }

  // placement new/delete
  static void* operator new(std::size_t size, void* ptr) throw()
  { return ::operator new(size, ptr); }
  static void operator delete(void* pMemory, void* ptr) throw()
  { ::operator delete(pMemory, ptr); }

  // nothrow new/delete
  static void* operator new(std::size_t size, const std::nothrow_t& nt) throw()
  { return ::operator new(size, nt); }
  static void operator delete(void* pMemory, const std::nothrow_t&) throw()
  { ::operator delete(pMemory); }
};

class WExp: public std::exception {
public:
  const char* what() const throw() {return "WExp";}
};

class Widget: public StandardNewDeleteForms {
public:
  using StandardNewDeleteForms::operator new;
  using StandardNewDeleteForms::operator delete;
  
  static void* operator new(std::size_t size, std::ostream& logStream) throw(std::bad_alloc)
  {
    logStream << "custom placement new" << std::endl;
    return ::operator new(size);
  }
  static void operator delete(void* pMemory, std::ostream& logStream) throw()
  {
    logStream << "custom placement delete" << std::endl;
    ::operator delete(pMemory);
  }

  Widget() {cout<<"ctor"<<endl;throw WExp();}
  ~Widget() {cout<<"dtor"<<endl;}
};

int main() {
  //Widget* pw1 = new Widget;
  //delete pw1; pw1 = NULL;

  try {
    Widget* pw2 = new(cout) Widget;
    delete pw2; pw2 = NULL;
  } catch(std::exception& exp) {
    cout << exp.what() << endl;
  }
}


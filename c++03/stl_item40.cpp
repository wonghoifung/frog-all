#include <list>
#include <iostream>
#include <algorithm>

using namespace std;

class Widget {
public:
  int id;
  Widget(int i): id(i) {}  
};

bool isInteresting(const Widget* pw) {
  return pw->id == 3;
}

// non-pointer types passed to unary_function or binary_function have consts and references stripped off
template <typename T>
class MeetsThreshold: public std::unary_function<Widget, bool> {
  const T threshold;
public:
  MeetsThreshold(const T& threshold) {}
  bool operator()(const Widget&) const {return true;}
};

struct WidgetNameCompare: std::binary_function<Widget, Widget, bool> {
  bool operator()(const Widget& lhs, const Widget& rhs) const {return true;}
};

// pointer types passed to unary_function or binary_function are the same types taken by operator()
struct PtrWidgetNameCompare: std::binary_function<const Widget*, const Widget*, bool> {
  bool operator()(const Widget* lhs, const Widget* rhs) const {
    return lhs->id < rhs->id;
  }
};

int main() {
  list<Widget*> widgetPtrs;
  widgetPtrs.push_back(new Widget(1));
  widgetPtrs.push_back(new Widget(2));
  widgetPtrs.push_back(new Widget(3));
  widgetPtrs.push_back(new Widget(4));
  widgetPtrs.push_back(new Widget(5));
  
  list<Widget*>::iterator i = find_if(widgetPtrs.begin(), widgetPtrs.end(), isInteresting);
  if (i != widgetPtrs.end()) cout << (*i)->id << endl;
  else cout << "nothing interesting" << endl;

  {
  list<Widget*>::iterator i = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(ptr_fun(isInteresting)));
  //list<Widget*>::iterator i = find_if(widgetPtrs.begin(), widgetPtrs.end(), not1(isInteresting));
  if (i != widgetPtrs.end()) cout << (*i)->id << endl;
  else cout << "nothing not interesting" << endl;
  }

  {
  Widget w(1);
  list<Widget*>::iterator i = find_if(widgetPtrs.begin(), widgetPtrs.end(), bind2nd(PtrWidgetNameCompare(), &w));
  if (i != widgetPtrs.end()) cout << (*i)->id << endl;
  else cout << "nothing precedes w" << endl;
  }
}


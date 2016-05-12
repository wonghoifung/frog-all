#include <deque>
#include <iostream>

int main() {
  typedef std::deque<int> IntDeque;
  typedef IntDeque::iterator Iter;
  typedef IntDeque::const_iterator ConstIter;
  IntDeque d;
  ConstIter ci;
  for (int i=0; i<100; ++i) d.push_back(i);
  ci = d.begin() + 50;
  std::cout<<*ci<<std::endl;  
  Iter i(d.begin());
  //std::advance(i,std::distance<Iter>(i,ci));
  std::advance(i,std::distance<ConstIter>(i,ci));
  std::cout<<*i<<std::endl;
}

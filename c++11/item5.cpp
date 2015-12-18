#include <unordered_map>
#include <iostream>

int main()
{
  std::unordered_map<int,int> m;
  m[1]=11;
  m[2]=22;
  for (const std::pair<int,int>& p:m)
  {
    // p is not reference within m
    //p.second += 1;
  }
  for (const auto& p:m)
  {
    std::cout<<p.first<<", "<<p.second<<std::endl;
  }
  return 0;
}


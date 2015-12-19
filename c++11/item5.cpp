#include <unordered_map>
#include <iostream>

using namespace std;

const std::pair<std::string,int>* getpp(std::unordered_map<std::string,int>& m)
{
  const std::pair<std::string,int>* pp = nullptr;
  for (const std::pair<std::string,int>& p:m)
  {
    // p is not reference within m
    if (p.second == 11) pp = &p;
  }
  //cout << __FUNCTION__ << ": " << pp->first << endl;
  return pp;
}

int main()
{
  std::unordered_map<std::string,int> m;
  m["1"]=11;
  m["2"]=22;
  const std::pair<std::string,int>* pp = nullptr;
  for (const std::pair<std::string,int>& p:m)
  //for (const auto& p:m)
  {
    // p is not reference within m
    //if (p.second == 11) pp = &p;
    ((pair<string,int>&)p).second += 1;
  }
  for (const auto& p:m)
  {
    std::cout<<p.first<<", "<<p.second<<std::endl;
  }
  cout << __FUNCTION__ << ": " << getpp(m)->first << endl;
  return 0;
}


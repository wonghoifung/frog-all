#ifndef CTESTADAPTER_HEADER
#define CTESTADAPTER_HEADER

#include "CTest.h"
#include "luaport.h"

class CTestAdapter : public CTest
{
public:
    int Add(lua_State* L)
    {
        int x = luaL_checknumber(L, -1);
        int y = luaL_checknumber(L, -2);
        //printf("in c file: x:%d, y:%d\n",x,y);
        int ret = CTest::Add(y, x);
        lua_pushnumber(L, ret);
        return 1;
    }
    
    int setx(lua_State* L)
    {
        int x = luaL_checknumber(L, -1);
        CTest::setx(x);
        return 0;
    }
    
    int getx(lua_State* L)
    {
        lua_pushnumber(L, CTest::getx());
        return 1;
    }
    
    static mfunc_item<CTestAdapter> mfuncs[4];
};

#endif


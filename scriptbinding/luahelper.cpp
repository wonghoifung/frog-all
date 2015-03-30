//
//  luahelper.cpp
//  luahelperprj
//
//  Created by wonghoifung on 15/3/24.
//  Copyright (c) 2015 wonghoifung. All rights reserved.
//

#include "luahelper.h"
#include <assert.h>

luahelper::luahelper(bool manualinit):L(NULL),manualinit_(manualinit)
{
   if(!manualinit_)init(); 
}

luahelper::~luahelper()
{
    if(!manualinit_)fini();
}

bool luahelper::init()
{
    if (L) {
        return false;
    }
    L = luaL_newstate();
    luaL_openlibs(L);
    return true;
}

bool luahelper::fini()
{
    if (L==NULL) {
        return false;
    }
    lua_close(L);
    L = NULL;
    return true;
}

void luahelper::bindfile(const char* fname)
{
    assert(L);
    dofile(L, fname);
}

std::string luahelper::get_global_string(const char* vname)
{
    std::string ret;
    lua_getglobal(L,vname);
    if(lua_isstring(L,-1)){
        ret = lua_tostring(L,-1);
    }
    lua_pop(L,1);
    return ret;
}

double luahelper::get_global_double(const char* dname)
{
    double ret = 0;
    lua_getglobal(L,dname);
    if(lua_isnumber(L,-1)){
        ret = lua_tonumber(L,-1);
    }
    lua_pop(L,1);
    return ret;
}

std::pair<std::string,double> luahelper::get_sdpair(const char* vname,
                                                    const char* dname)
{
    std::string s;
    double d(0);

    lua_getglobal(L,vname);
    lua_getglobal(L,dname);
    
    if(lua_isnumber(L,-1)){
        d = lua_tonumber(L,-1);
    }

    if(lua_isstring(L,-2)){
        s = lua_tostring(L,-2);
    }
    
    lua_pop(L,2);
    return std::make_pair(s,d);
}

std::string luahelper::get_field_string(const char* cname, const char* fname)
{
    std::string ret;
    lua_getglobal(L,cname);
    lua_getfield(L,-1,fname);
    if(lua_isstring(L,-1)){
        ret = lua_tostring(L,-1);
    }
    lua_pop(L,2);
    return ret;
}

double luahelper::get_field_double(const char* cname, const char* fname)
{
    double ret = 0;
    lua_getglobal(L,cname);
    lua_getfield(L,-1,fname);
    if(lua_isnumber(L,-1)){
        ret = lua_tonumber(L,-1);
    }
    lua_pop(L,2);
    return ret;
}

void luahelper::call_func(const char* fname)
{
    lua_getglobal(L,fname);
    lua_call(L,0,0);//0 para 0 reval
    //no need to clear lua stack
}

std::string luahelper::call_func(const char* fname,
                                 const char* arg1,
                                 const char* arg2)
{
    std::string ret;
    lua_getglobal(L,fname);
    lua_pushstring(L,arg1);
    lua_pushstring(L,arg2);
    lua_call(L,2,1);
    if(lua_isstring(L,-1)){
        ret = lua_tostring(L,-1);
    }
    lua_pop(L,1);
    return ret;
}

void luahelper::call_func(const char* fname, int n)
{
    lua_getglobal(L,fname);
    lua_pushnumber(L,n);
    lua_call(L,1,0);
}

lua_State* luahelper::get_lua_state()
{
    return L;
}


//
//  luahelper.h
//  luahelperprj
//
//  Created by wonghoifung on 15/3/24.
//  Copyright (c) 2015 wonghoifung. All rights reserved.
//

#ifndef luahelper_header
#define luahelper_header

#include <lua.hpp>
#include <map>
#include <string>
#include <boost/noncopyable.hpp>

extern "C"
{
int dofile (lua_State *L, const char *name);
};

class luahelper  : public boost::noncopyable
{
public:
    luahelper();
    ~luahelper();
    
    bool init();
    bool fini();
    void bindfile(const char* fname);
    
    std::string get_global_string(const char* vname);
    double get_global_double(const char* dname);

    std::pair<std::string,double> get_sdpair(const char* vname, const char* dname);

    std::string get_field_string(const char* cname, const char* fname);
    double get_field_double(const char* cname, const char* fname);
   
    void call_func(const char* fname); 
    std::string call_func(const char* fname, const char* arg1, const char* arg2);

    lua_State* get_lua_state();

private:
    lua_State* L;
};

template <typename T>
void lua_pushpointer(lua_State* L, T* p) {
    T** ud=(T**)lua_newuserdata(L,sizeof(T*));
    *ud = p;
}

#endif



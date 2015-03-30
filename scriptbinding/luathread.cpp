//
//  luathread.cpp
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014 wonghoifung. All rights reserved.
//

#include "luahelper.h"
#include "luathread.h"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

typedef frog::utils::mrmw_queue<NETInputPacket*> ___qt;
 
static int getmsg(lua_State* L) {
    //___qt** pq = (___qt**)lua_topointer(L, lua_upvalueindex(1));
    //___qt* packs_ = *pq;
    ___qt* packs_ = (___qt*)lua_topointer(L, lua_upvalueindex(1));

    if (packs_) {
        NETInputPacket* inp = packs_->pop();
        lua_inpack* ret = new lua_inpack();
        ret->NETInputPacket::Copy(inp->packet_buf(), inp->packet_size());
        *(lua_inpack**)lua_newuserdata(L,sizeof(lua_inpack*)) = ret;
        luaL_getmetatable(L, "NETInputPacket");
        lua_setmetatable(L, -2);
        delete inp;
        return 1;
    }
    return 0;
}

static int luasleep(lua_State* L) {
    int s = (int)lua_tonumber(L, -1);
    sleep(s);
    return 0;
}

static int cppsetmsgcnt(lua_State* L) {
    frog::utils::luathread* lt = (frog::utils::luathread*)lua_topointer(L, lua_upvalueindex(1));
    if (lt) {
        int mcnt = (int)lua_tonumber(L,-1);
        lt->setmsgcnt(mcnt);
    }
    return 0;
}

static int cppgetmsgcnt(lua_State* L) {
    frog::utils::luathread* lt = (frog::utils::luathread*)lua_topointer(L, lua_upvalueindex(1));
    if (lt) {
        int ret = lt->getmsgcnt();
        lua_pushnumber(L,ret);
        return 1;
    }
    return 0;
}

namespace frog
{
    namespace utils
    {
       
        luathread::luathread() : stop_(false),msgcnt_(0),initdone_(1),luahelp_(true)
        {
        }
        
        void luathread::init()
        {
            thread_ = boost::make_shared<thread_t>(boost::bind(&luathread::run, this));
            initdone_.wait();
        }
        
        void luathread::pushmsg(NETInputPacket* pack)
        {
            packs_.push(pack);
        }
       
        void luathread::run()
        {
            while(!stop_) {
                //luahelper lh;
                luahelper& lh = luahelp_;
                lh.init();

                lua_State* L = lh.get_lua_state();

                //lua_pushpointer(L, &packs_);
                lua_pushlightuserdata(L, &packs_);
                lua_pushcclosure(L, getmsg, 1);
                lua_setglobal(L, "getmsg");
            
                lua_pushlightuserdata(L, this);
                lua_pushcclosure(L, cppsetmsgcnt, 1);
                lua_setglobal(L, "cppsetmsgcnt");

                lua_pushlightuserdata(L, this);
                lua_pushcclosure(L, cppgetmsgcnt, 1);
                lua_setglobal(L, "cppgetmsgcnt");

                lua_register(L, "sleep", luasleep);
    
                luaport<lua_inpack>::register_class(L, "NETInputPacket");
                luaport<lua_outpack>::register_class(L, "NETOutputPacket");

                initdone_.done();
            
                lh.bindfile("./luascript/app.lua");

                lh.fini();
            }
        }
        
    }
}


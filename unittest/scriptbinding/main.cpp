//
//  main.cpp
//  luahelperprj
//
//  Created by wonghoifung on 15/3/24.
//  Copyright (c) 2015 wonghoifung. All rights reserved.
//

enum {
    cmd_login = 1,
    cmd_logout = 2,
    cmd_client_say = 3,
};

#if 1
#include "luathread.h"

using std::cout;
using std::endl;
using frog::utils::luathread;

NETInputPacket* fakemessage(int cmd, int mid) {
    NETOutputPacket out;
    switch (cmd) {
        case cmd_login:
        {
            out.Begin(cmd_login);
            out.WriteInt(mid);
            out.WriteString("albert");//pwd
            out.End();
            break;
        }
        case cmd_logout:
        {
            out.Begin(cmd_logout);
            out.WriteInt(mid);
            out.End();
            break;
        }
        case cmd_client_say:
        {
            out.Begin(cmd_client_say);
            out.WriteInt(mid);
            out.WriteString("hello universe");
            out.End();
            break;
        }
        default:
            assert(false);
            return NULL;
    }
    NETInputPacket* in = new NETInputPacket();
    in->Copy(out.packet_buf(), out.packet_size());
    return in;
}

int main(int argc, const char * argv[]) {

    luathread t;
    t.init();

    t.pushmsg(fakemessage(cmd_login, 1));
    t.pushmsg(fakemessage(cmd_login, 2));
    t.pushmsg(fakemessage(cmd_login, 3));
    
    t.pushmsg(fakemessage(cmd_client_say, 3));
    t.pushmsg(fakemessage(cmd_client_say, 2));
    t.pushmsg(fakemessage(cmd_client_say, 1));
    
    t.pushmsg(fakemessage(cmd_logout, 2));
    t.pushmsg(fakemessage(cmd_logout, 1));
    t.pushmsg(fakemessage(cmd_logout, 3));


    t.join();
    return 0;
}

#endif

#if 0
#include <iostream>
#include <sstream>
#include <list>
#include <assert.h>
#include "luahelper.h"
#include "PacketBaseAdapter.h"

using std::cout;
using std::endl;

std::list<NETInputPacket*> msgqueue;

NETInputPacket* fakemessage(int cmd, int mid) {
    NETOutputPacket out;
    switch (cmd) {
        case cmd_login:
        {
            out.Begin(cmd_login);
            out.WriteInt(mid);
            out.WriteString("albert");//pwd
            out.End();
            break;
        }
        case cmd_logout:
        {
            out.Begin(cmd_logout);
            out.WriteInt(mid);
            out.End();
            break;
        }
        case cmd_client_say:
        {
            out.Begin(cmd_client_say);
            out.WriteInt(mid);
            out.WriteString("hello universe");
            out.End();
            break;
        }
        default:
            assert(false);
            return NULL;
    }
    NETInputPacket* in = new NETInputPacket();
    in->Copy(out.packet_buf(), out.packet_size());
    return in;
}

void fill_fakemessages() {
    msgqueue.push_back(fakemessage(cmd_login, 1));
    msgqueue.push_back(fakemessage(cmd_login, 2));
    msgqueue.push_back(fakemessage(cmd_login, 3));
    
    msgqueue.push_back(fakemessage(cmd_client_say, 3));
    msgqueue.push_back(fakemessage(cmd_client_say, 2));
    msgqueue.push_back(fakemessage(cmd_client_say, 1));
    
    msgqueue.push_back(fakemessage(cmd_logout, 2));
    msgqueue.push_back(fakemessage(cmd_logout, 1));
    msgqueue.push_back(fakemessage(cmd_logout, 3));
}

int getmsg(lua_State* L) {
    // TODO use cond var
    if (msgqueue.size()) {
        NETInputPacket* inp = msgqueue.front();
        msgqueue.pop_front();
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

int luasleep(lua_State* L) {
    int s = (int)lua_tonumber(L, -1);
    sleep(s);
    return 0;
}

int main(int argc, const char * argv[]) {
    luahelper lh;
    lua_State* L = lh.get_lua_state();

    lua_register(L, "getmsg", getmsg);
    lua_register(L, "sleep", luasleep);
    
    luaport<lua_inpack>::register_class(L, "NETInputPacket");
    luaport<lua_outpack>::register_class(L, "NETOutputPacket");

    fill_fakemessages();
    
    lh.bindfile("./luascript/app.lua");
   
    cout << "-------- test global and field" << endl; 
    cout << lh.get_global_string("name") << endl;
    cout << lh.get_global_double("version") << endl;
    cout << lh.get_field_double("people", "age") << endl;
    cout << lh.get_field_string("people", "name") << endl;

    cout << "-------- test pair" << endl; 
    std::pair<std::string,double> r = lh.get_sdpair("name","version");
    cout << r.first << " " << r.second << endl;

    cout << "-------- test function" << endl; 
    cout << lh.call_func("getstr","aaa","bbb") << endl;
    lh.call_func("main");
    
    return 0;
}
#endif




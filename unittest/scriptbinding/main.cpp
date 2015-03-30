//
//  main.cpp
//  luahelperprj
//
//  Created by wonghoifung on 15/3/24.
//  Copyright (c) 2015 wonghoifung. All rights reserved.
//

#include "luathread.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using frog::utils::luathread;

enum {
    cmd_stoploop = 0,
    cmd_login = 1,
    cmd_logout = 2,
    cmd_client_say = 3,
};

NETInputPacket* buildinpack(int cmd, int mid=0, const string& s="") {
    NETOutputPacket out;
    switch (cmd) {
        case cmd_stoploop:
        {
            out.Begin(cmd_stoploop);
            out.End();
            break;
        }
        case cmd_login:
        {
            out.Begin(cmd_login);
            out.WriteInt(mid);
            out.WriteString(s);//pwd
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
            out.WriteString(s);//content
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
   
    int startts = time(NULL); 
    for(int i=0; i<10000; ++i){        
        t.pushmsg(buildinpack(1,123,"pwd123"));
        t.pushmsg(buildinpack(1,234,"pwd234"));
        t.pushmsg(buildinpack(1,345,"pwd345"));
 
        t.pushmsg(buildinpack(3,345,"content345"));
        t.pushmsg(buildinpack(3,234,"content234"));
        t.pushmsg(buildinpack(3,123,"content123"));

        t.pushmsg(buildinpack(2,123));
        t.pushmsg(buildinpack(2,234));
        t.pushmsg(buildinpack(2,345));
    
        t.pushmsg(buildinpack(3,345,"content345"));
        t.pushmsg(buildinpack(3,234,"content234"));
        t.pushmsg(buildinpack(3,123,"content123"));
    
        t.pushmsg(buildinpack(0));
    }

    while(t.getmsgcnt()<130000) {
        //...
    }
    int endts = time(NULL);
    cout << "all done, cost " << (endts-startts) << " seconds" << endl;

#if 0
    int cmd=-1;
    int mid=0;
    string info;

    while(cin>>cmd) {
        switch(cmd) {
        case cmd_stoploop:
            t.pushmsg(buildinpack(cmd));
            break;
        case cmd_login:
            cin>>mid;
            cin>>info;
            t.pushmsg(buildinpack(cmd,mid,info));
            break;
        case cmd_logout:
            cin>>mid;
            t.pushmsg(buildinpack(cmd,mid));
            break;
        case cmd_client_say:
            cin>>mid;
            cin>>info;
            t.pushmsg(buildinpack(cmd,mid,info));
            break;
        case 999:
            t.pushmsg(buildinpack(cmd_stoploop));
            t.setstop(true);
            break;
        default:
            cout << "error: cmd not valid" << endl;
            break;
        }
        if (cmd==999) break;
    }
#endif
    t.join();
    return 0;
}






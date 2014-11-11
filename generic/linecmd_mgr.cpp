//
//  linecmd_mgr.cpp
//  console
//
//  Created by huanghaifeng on 14-10-4.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "linecmd_mgr.h"

namespace frog
{
    namespace generic
    {
        
        linecmd_mgr& linecmd_mgr::ref()
        {
            static linecmd_mgr instance;
            return instance;
        }
        
        bool linecmd_mgr::addlcmd(const char* cmd, lcommand_callback cb)
        {
            return cmd_cb_lut_.insert(std::make_pair(cmd, cb)).second;
        }
        
        void linecmd_mgr::dellcmd(const char* cmd)
        {
            cmd_cb_lut_.erase(cmd);
        }
        
        bool linecmd_mgr::dispatch(std::string& lcmd,
                                   std::vector<std::string>& args,
                                   tcpsession_ptr session)
        {
            std::map<std::string, lcommand_callback>::iterator it = cmd_cb_lut_.find(lcmd);
            if (it != cmd_cb_lut_.end())
            {
                lcommand_callback& callback = it->second;
                callback(args, session);
                return true;
            }
            
            if (lcmd == "help")
            {
                help(session);
                return true;
            }
            
            std::string info = lcmd;
            info += ": command not found\n";
            session->send_line( info.c_str(), (int)(info.size()) );
            return false;
        }
        
        void linecmd_mgr::help(tcpsession_ptr session)
        {
            std::map<std::string, lcommand_callback>::iterator beg(cmd_cb_lut_.begin()), end(cmd_cb_lut_.end());
            std::stringstream ss;
            ss << "commands: ";
            for (; beg != end; ++beg)
            {
                ss << beg->first << " ";
            }
            ss << "\n";
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
    }
}

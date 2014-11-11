//
//  linecmd_mgr.h
//  console
//
//  Created by huanghaifeng on 14-10-4.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef linecmd_mgr_header
#define linecmd_mgr_header

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <string>
#include <vector>
#include <map>
#include "tcpsession.h"

namespace frog
{
    namespace generic
    {
        
        typedef boost::function<void(std::vector<std::string>&,tcpsession_ptr)> lcommand_callback;
        
        class linecmd_mgr : public boost::noncopyable
        {
        public:
            static linecmd_mgr& ref();
            bool addlcmd(const char*, lcommand_callback);
            void dellcmd(const char*);
            bool dispatch(std::string& lcmd, std::vector<std::string>& args, tcpsession_ptr session);
            
        private:
            linecmd_mgr(){}
            ~linecmd_mgr(){}
            void help(tcpsession_ptr session);
            
        private:
            std::map<std::string, lcommand_callback> cmd_cb_lut_;
        };
        
    }
}

#endif

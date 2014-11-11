//
//  config.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef config_header
#define config_header

#include <string>
#include <map>
#include <boost/noncopyable.hpp>

namespace frog
{
    namespace proxy
    {
        
        struct command_range
        {
            int min;
            int max;
            std::string tag;
        };
        
        class config : public boost::noncopyable
        {
        private:
            config();
            ~config();
            
        public:
            static config& ref();
            bool init(const std::string& cfgfile);
            void dump();
            
            int get_port() {return port_;}
            int get_console_port() {return console_port_;}
            std::string get_ip() {return ip_;}
            std::string get_name() {return name_;}
            std::map<int, command_range>& get_cmdrange() {return cmdrange_;}
            
            
        private:
            int port_;
            int console_port_;
            std::string ip_;
            std::string name_;
            std::map<int, command_range> cmdrange_;
        };
        
    }
}

#endif

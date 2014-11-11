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
#include <sstream>
#include <map>
#include <boost/noncopyable.hpp>

namespace frog
{
    namespace chat
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
            
            int get_console_port() {return console_port_;}
            std::string get_ip() {return ip_;}
            std::string get_name() {return name_;}
            int get_servertype() {return servertype_;}
            std::string get_proxy_ip() {return proxy_ip_;}
            int get_proxy_port() {return proxy_port_;}
            std::string get_proxy_port_str()
            {
                std::stringstream ss;
                ss << proxy_port_;
                return ss.str();
            }
            
        private:
            int console_port_;
            std::string ip_;
            std::string name_;
            int servertype_;
            std::string proxy_ip_;
            int proxy_port_;
        };
        
    }
}

#endif

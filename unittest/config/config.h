//
//  config.h
//  console
//
//  Created by huanghaifeng on 14-11-6.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#if 0

#ifndef config_header
#define config_header

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

/* config template */

namespace frog
{
    namespace utils
    {
        
        class config : public boost::noncopyable
        {
        private:
            config();
            ~config();
            
        public:
            static config& ref();
            bool init(const std::string& cfgfile);
            
            int get_port() {return port_;}
            int get_console_port() {return console_port_;}
            std::string get_ip() {return ip_;}
            std::string get_name() {return name_;}
            std::vector<std::string>& get_urls() {return urls_;}
            
            int get_imgid() {return imgid_;}
            std::vector<std::string>& get_imgs() {return imgs_;}
            std::string get_newimgs() {return newimgs_;}
            
        private:
            int parse_json();
            int insert_json();
            
        private:
            int port_;
            int console_port_;
            std::string ip_;
            std::string name_;
            std::vector<std::string> urls_;
            
            int imgid_;
            std::vector<std::string> imgs_;
            std::string newimgs_;
        };
        
    }
}

#endif 

#endif

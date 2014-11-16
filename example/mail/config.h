//
//  config.h
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef config_header
#define config_header

#include <string>
#include <sstream>
#include <map>
#include <boost/noncopyable.hpp>
#include "dboperator.h"

namespace frog
{
    namespace mail
    {
        
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
            
            int get_db_type() {return db_type_;}
            db::conn_args_t& get_db_connargs() {return db_connargs_;}
            std::string get_defaultdb() {return defaultdb_;}
            
            int get_mailtable_count() {return mail_table_count_;}
            std::string get_mailtable_name() {return mail_table_name_;}
            std::string get_mailtable_db() {return mail_table_db_;}
            
        private:
            int console_port_;
            std::string ip_;
            std::string name_;
            int servertype_;
            
            std::string proxy_ip_;
            int proxy_port_;
            
            int db_type_;
            db::conn_args_t db_connargs_;
            std::string defaultdb_;
            
            int mail_table_count_;
            std::string mail_table_name_;
            std::string mail_table_db_;
        };
        
    }
}

#endif

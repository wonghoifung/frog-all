//
//  config.cpp
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "config.h"
#include "logger.h"
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

namespace frog
{
    namespace mail
    {
        
        config::config()
        {
            
        }
        
        config::~config()
        {
            
        }
        
        config& config::ref()
        {
            static config cfg;
            return cfg;
        }
        
        bool config::init(const std::string& cfgfile)
        {
            using namespace boost::property_tree;
            
            try
            {
                ptree pt;
                
                read_xml(cfgfile,pt);
                
                console_port_ = pt.get<int>("frog.consoleport");
                ip_ = pt.get<std::string>("frog.ip","127.0.0.1");
                name_ = pt.get<std::string>("frog.name");
                servertype_ = pt.get<int>("frog.srvtype");
                
                proxy_ip_ = pt.get<std::string>("frog.proxy.ip");
                proxy_port_ = pt.get<int>("frog.proxy.port");
                
                db_type_ = pt.get<int>("frog.db.type");
                db_connargs_.host = pt.get<std::string>("frog.db.ip");
                db_connargs_.port = pt.get<std::string>("frog.db.port");
                db_connargs_.user = pt.get<std::string>("frog.db.usr");
                db_connargs_.passwd = pt.get<std::string>("frog.db.pwd");
                defaultdb_ = pt.get<std::string>("frog.db.defaultdb");
                
                mail_table_count_ = pt.get<int>("frog.mailtable.count");
                mail_table_name_ = pt.get<std::string>("frog.mailtable.name");
                mail_table_db_ = pt.get<std::string>("frog.mailtable.db");
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << "\n";
                return false;
            }
            
            return true;
        }
        
        void config::dump()
        {
            logstr("consoleport: %d",console_port_);
            logstr("ip: %s",ip_.c_str());
            logstr("name: %s",name_.c_str());
            logstr("server type: %d",servertype_);
            
            logstr("proxy ip: %s",proxy_ip_.c_str());
            logstr("proxy port: %d",proxy_port_);
            
            logstr("db type: %d", db_type_);
            logstr("db host: %s", db_connargs_.host.c_str());
            logstr("db port: %s", db_connargs_.port.c_str());
            logstr("db usr: %s", db_connargs_.user.c_str());
            logstr("db pwd: %s", db_connargs_.passwd.c_str());
            logstr("default db: %s", defaultdb_.c_str());
            
            logstr("mail table count: %d", mail_table_count_);
            logstr("mail table name: %s", mail_table_name_.c_str());
            logstr("mail table db: %s", mail_table_db_.c_str());
        }
        
    }
}

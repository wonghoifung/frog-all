//
//  config.cpp
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "config.h"
#include "logger.h"
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

namespace frog
{
    namespace chat
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
        }
        
    }
}


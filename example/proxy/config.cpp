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
    namespace proxy
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
                
                port_ = pt.get<int>("frog.port");
                
                console_port_ = pt.get<int>("frog.consoleport");
                
                ip_ = pt.get<std::string>("frog.ip","127.0.0.1");
                
                name_ = pt.get<std::string>("frog.name");
                
                ptree child = pt.get_child("frog.commands");
                
                cmdrange_.clear();
                for(BOOST_AUTO(pos,child.begin()); pos != child.end(); ++pos)
                {
                    command_range rng;
                    int srvtype = pos->second.get<int>("<xmlattr>.srvtype");
                    rng.min = pos->second.get<int>("<xmlattr>.min");
                    rng.max = pos->second.get<int>("<xmlattr>.max");
                    rng.tag = pos->second.get<std::string>("<xmlattr>.tag");
                    cmdrange_[srvtype] = rng;
                }
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
            logstr("port: %d", port_);
            logstr("consoleport: %d", console_port_);
            logstr("ip: %s",ip_.c_str());
            logstr("name: %s",name_.c_str());
            std::map<int, command_range>::iterator beg(cmdrange_.begin()),end(cmdrange_.end());
            for (; beg!=end; ++beg)
            {
                command_range& rng = beg->second;
                logstr("id:%d, tag:%s, min:%d, max:%d",beg->first,rng.tag.c_str(),rng.min,rng.max);
            }
        }
        
    }
}


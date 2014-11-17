//
//  config.cpp
//  console
//
//  Created by huanghaifeng on 14-11-6.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#if 0

#include "config.h"
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

namespace frog
{
    namespace utils
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
                
                ptree child = pt.get_child("frog.urls");
                
                int size = pt.get<int>("frog.urls.<xmlattr>.size"); (void)size;
                
                for(BOOST_AUTO(pos,child.begin()); pos != child.end(); ++pos)
                {
                    if(pos->second.data().size())urls_.push_back(pos->second.data());
                }
                
                //pt.put("frog.name","frog_client"); /* modify */
                //pt.add("frog.urls.url","https://github.com/wonghoifung/frog-erlang.git"); /* add */
                //write_xml(cfgfile,pt);
                
                if (parse_json()!=0)
                {
                    return false;
                }
                
                if (insert_json()!=0)
                {
                    return false;
                }

            }
            catch (std::exception& e)
            {
                //std::cerr << e.what() << "\n";
                return false;
            }
            
            return true;
        }
        
        int config::parse_json()
        {
            using namespace boost::property_tree;
            
            //std::string str = "{\"id\":0,\"images\":[{\"url\":\"img/cat.jpg\"},{\"url\":\"img/dog.jpg\"}]}";
            
            std::string str;
            std::fstream f("/Users/huanghaifeng/Desktop/frog/config/example.json");
            while (f)
            {
                std::string line;
                f >> line;
                str.append(line);
            }
            
            std::stringstream ss(str);
            
            ptree pt;
            try
            {
                read_json(ss, pt);
            }
            catch(ptree_error& e)
            {
                //std::cerr << e.what() << "\n";
                return 1;
            }
            
            try
            {
                imgid_ = pt.get<int>("id");
                ptree image_array = pt.get_child("images");
                
                BOOST_FOREACH(boost::property_tree::ptree::value_type& v, image_array)
                {
                    //std::stringstream s;
                    //write_json(s, v.second);
                    //std::string image_item = s.str();
                    //std::cout << image_item << std::endl;
                    std::string item = v.second.get<std::string>("url");
                    if(item.size())imgs_.push_back(item);
                }  
            }  
            catch (ptree_error& e)  
            {  
                //std::cerr << e.what() << "\n";
                return 2;  
            }  
            return 0;
        }
        
        int config::insert_json()
        {
            using namespace boost::property_tree;
            
            std::string str = "{\"id\":0,\"images\":[{\"url\":\"img/cat.jpg\"},{\"url\":\"img/dog.jpg\"}]}";
            
            std::stringstream ss(str);
            ptree pt;
            try
            {
                read_json(ss, pt);
            }
            catch(ptree_error& e)
            {
                //std::cerr << e.what() << "\n";
                return 1;
            }
            
            //std::cout << pt.get<int>("id") << std::endl;
            
            // modify or add
            pt.put("oid", "007");
            
            // add array
            ptree exif_array;
            ptree node1, node2, node3;
            node1.put("key1", "value1");
            node2.put("key2", "value2");
            node3.put("key3", "value3");
            exif_array.push_back(std::make_pair("", node1));
            exif_array.push_back(std::make_pair("", node2));
            exif_array.push_back(std::make_pair("", node3));    
            pt.put_child("grp", exif_array);  
            
            std::stringstream s2;  
            write_json(s2, pt);  
            newimgs_ = s2.str();
            //std::cout << outstr << std::endl;
            
            return 0;
        }
        
    }
}

#endif


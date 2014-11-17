//
//  main.cpp
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "config.h"
#include "proxy_client_handler.h"
#include "console_server_handler.h"
#include "hash_tag.h"
#include "logger.h"
#include "memcached_manager.h"

int main()
{
    using namespace frog::mail;
    
    /* logger */
    if (!frog::utils::logger::ref().init("mail", "./"))
    {
        std::cerr << "init logger error\n";
        return -1;
    }
    
    /* config file */
    if (!config::ref().init("./frog_cfg.xml"))
    {
        logstr("load config error");
        return -1;
    }
    config::ref().dump();
    
    /* db */
    frog::db::dboperator db(frog::db::dbtype_cast(config::ref().get_db_type()));
	if (!db.connect(config::ref().get_db_connargs()))
	{
        logstr("connect db failed");
        return -1;
    }
    global_dboperator.set(&db);
    
    /* cache */
    frog::utils::hash_tag hashtag;
    frog::db::dataset* ds = db.query("select * from configs.memcached_address;");
    for (unsigned long i=0; i<ds->row_count(); ++i)
    {
        int tag = ds->get_field_int32(i, "cache_tag");
        std::string ip = ds->get_field_string(i, "cache_ip");
        int port = ds->get_field_int32(i, "cache_port");
        if (!frog::cache::memcached_manager::ref().add_cache(tag, ip, port))
        {
            logstr("add cache failed");
            return -1;
        }
        logstr("cache tap:%d ip:%s port:%d", tag, ip.c_str(), port);
        hashtag.add_tag(tag);
    }
    global_hash_tag.set(&hashtag);
    
    try
    {
        boost::asio::io_service io_service;
        global_io_service.set(&io_service);
        
        /* connect to proxy */
        frog::generic::tcpclient proxyclient(io_service);
        frog::mail::proxy_client_handler proxyclienthandler;
        proxyclienthandler.installcb(proxyclient);
        proxyclient.connect(config::ref().get_proxy_ip().c_str(),
                            config::ref().get_proxy_port_str().c_str());
        
        /* monitor console */
		frog::generic::tcpserver consoleserver(io_service,
                                               config::ref().get_console_port(),
                                               frog::generic::parse_line);
        frog::console::console_server_handler consolehandler;
        consolehandler.installcb(consoleserver);
        consoleserver.run();
        
        io_service.run();
    }
    catch (std::exception& e)
    {
        logstr("exception: %s",e.what());
    }
}

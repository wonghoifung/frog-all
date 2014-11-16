//
//  main.cpp
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "config.h"
#include "proxy_server_handler.h"
#include "console_server_handler.h"
#include "utils/global_holder.h"
#include "logger.h"
#include "pid_file.h"

int main()
{
    using namespace frog::proxy;
    
    frog::utils::create_pid_file();
    
    if (!frog::utils::logger::ref().init("proxy", "./"))
    {
        std::cerr << "init logger error\n";
        return -1;
    }
    
    if (!config::ref().init("./frog_cfg.xml"))
    {
        logstr("load config error");
        return -1;
    }
    
    config::ref().dump();
    
    try
    {
        boost::asio::io_service io_service;
        frog::utils::global_holder<boost::asio::io_service>::ref().set(&io_service);
    
        frog::generic::tcpserver pserver(io_service,
                                         config::ref().get_port(),
                                         frog::generic::parse_pack);
        proxy_server_handler pshandler;
        pshandler.installcb(pserver);
        pserver.run();
        
	frog::generic::tcpserver consoleserver(io_service,
                                               config::ref().get_console_port(),
                                               frog::generic::parse_line);
        frog::console::console_server_handler chandler;
        chandler.installcb(consoleserver);
        consoleserver.run();
        
        io_service.run();
    }
    catch (std::exception& e)
    {
        logstr("exception: %s",e.what());
    }
}


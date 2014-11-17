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
#include "proxy_client_handler.h"
#include "console_server_handler.h"
#include "utils/global_holder.h"
#include "logger.h"
#include "pid_file.h"
#include "timer.h"
#include "time_utils.h"

int main()
{
    using namespace frog::chat;
    
    frog::utils::create_pid_file();
    
    if (!frog::utils::logger::ref().init("chat", "./"))
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
        
        frog::generic::tcpclient proxyclient(io_service);
        frog::chat::proxy_client_handler proxyclienthandler;
        frog::utils::global_holder<frog::chat::proxy_client_handler>::ref().set(&proxyclienthandler);
        proxyclienthandler.installcb(proxyclient);
        proxyclient.connect(config::ref().get_proxy_ip().c_str(),
                            config::ref().get_proxy_port_str().c_str());
        
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


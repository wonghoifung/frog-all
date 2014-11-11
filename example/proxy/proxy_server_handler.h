//
//  proxy_server_handler.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef proxy_server_handler_header
#define proxy_server_handler_header

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "generic/tcpserver.h"
#include "generic/linecmd_mgr.h"
#include "time_utils.h"
#include "dispatcher.h"

namespace frog
{
    namespace proxy
    {
        
        class proxy_server_handler : public boost::noncopyable
        {
        public:
            proxy_server_handler();
            void lcmd_stat(std::vector<std::string>&,generic::tcpsession_ptr session);
            void lcmd_clear(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_reload(std::vector<std::string>&,generic::tcpsession_ptr);
            void installcb(generic::tcpserver& s);
            int on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            void on_close(frog::generic::tcpsession_ptr session);
            void on_connect(frog::generic::tcpsession_ptr session);
            void on_timeout(frog::generic::tcpsession_ptr session);
            void on_error(frog::generic::tcpsession_ptr session, int errcode);
            
        private:
            void clear_stat();
            
        private:
            int msgcount_;
            int conncount_;
            int closecount_;
            int timeoutcount_;
            int errorcount_;
            time_t starttime_;
            dispatcher dispatcher_;
        };
        
    }
}

#endif 

//
//  proxy_client_handler.h
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef proxy_client_handler_header
#define proxy_client_handler_header

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "generic/tcpclient.h"
#include "utils/global_holder.h"
#include "mail_manager.h"

namespace frog
{
    namespace mail
    {
        
        struct mail_stat
        {
            int reconn_count_;
            int receive_msgcount_;
            mail_stat() {clear();}
            void clear()
            {
                reconn_count_ = 0;
                receive_msgcount_ = 0;
            }
        };
        
        // TODO reconnect when broken
        class proxy_client_handler : boost::noncopyable
        {
        public:
            proxy_client_handler();
            void lcmd_mail_stat(std::vector<std::string>&,generic::tcpsession_ptr session);
            void lcmd_mail_clear(std::vector<std::string>&,generic::tcpsession_ptr session);
            
            void installcb(generic::tcpclient& client);
            int on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            void on_close(frog::generic::tcpsession_ptr session);
            void on_connect(frog::generic::tcpsession_ptr session);
            void on_error(frog::generic::tcpsession_ptr session, int errcode);
            
        private:
            mail_manager mgr_;
            mail_stat stat_;
        };
        
    }
}

#endif

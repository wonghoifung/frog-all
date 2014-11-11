//
//  tcpclient.h
//  console
//
//  Created by huanghaifeng on 14-10-2.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef tcpclient_header
#define tcpclient_header

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include "tcpsession.h"

namespace frog
{
    namespace generic
    {
        
        class tcpclient : boost::noncopyable
        {
        public:
            tcpclient(boost::asio::io_service& iosrv);
            void connect(const char* ip, const char* port);
            void close();
            int on_message(tcpsession_ptr session, decoder* pack);
            void set_messagecb(message_callback cb) { msgcb_ = cb; }
            void set_closecb(close_callback cb) { closecb_ = cb; }
            void set_connectcb(connect_callback cb) { connectcb_ = cb; }
            void set_errorcb(error_callback cb) { errorcb_ = cb; }
            
        private:
            void handle_connect(const boost::system::error_code& error,
                                boost::asio::ip::tcp::resolver::iterator it,
                                tcpsession_ptr session);
            
        private:
            boost::asio::io_service& iosrv_;
            tcpsession_ptr session_;
            message_callback msgcb_;
            close_callback closecb_;
            connect_callback connectcb_;
            error_callback errorcb_;
        };
        
    }
}

#endif

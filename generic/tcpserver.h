//
//  tcpserver.h
//  console
//
//  Created by huanghaifeng on 14-10-2.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef tcpserver_header
#define tcpserver_header

#include "tcpsession.h"

namespace frog
{
    namespace generic
    {
        
        class tcpserver : public boost::noncopyable
        {
        public:
            tcpserver(boost::asio::io_service& io_service, short port, parser_type ptype);
            void run();
            void set_messagecb(message_callback cb) { messagecb_ = cb; }
            void set_closecb(close_callback cb) { closecb_ = cb; }
            void set_connectcb(connect_callback cb) { connectcb_ = cb; }
            void set_errorcb(error_callback cb) { errorcb_ = cb; }
            void set_timeoutcb(timeout_callback cb) { timeoutcb_ = cb; }
            
        private:
            void start_accept();
            void handle_accept(tcpsession_ptr new_session, const boost::system::error_code& error);
            
        private:
            boost::asio::io_service& io_service_;
            boost::asio::ip::tcp::acceptor acceptor_;
            message_callback messagecb_;
            close_callback closecb_;
            connect_callback connectcb_;
            error_callback errorcb_;
            timeout_callback timeoutcb_;
            parser_type parsetype_;
        };
        
    }
}

#endif

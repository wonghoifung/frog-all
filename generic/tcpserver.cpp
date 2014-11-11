//
//  tcpserver.cpp
//  console
//
//  Created by huanghaifeng on 14-10-2.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include "tcpserver.h"
#include "tcpsessionmgr.h"

using boost::asio::ip::tcp;

namespace frog
{
    namespace generic
    {
        
        tcpserver::tcpserver(boost::asio::io_service& io_service, short port, parser_type ptype)
            : io_service_(io_service),
              acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
              parsetype_(ptype)
        {
        }
        
        void tcpserver::run()
        {
            tcpsessionmgr::ref().set_errorcb(errorcb_);
            tcpsessionmgr::ref().set_timeoutcb(timeoutcb_);
            start_accept();
        }
        
        void tcpserver::start_accept()
        {
            tcpsession_ptr new_session = boost::make_shared<tcpsession>(io_service_, tcpsession_server, parsetype_);
            new_session->set_messagecb(messagecb_);
            new_session->set_closecb(closecb_);
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&tcpserver::handle_accept,
                                               this,
                                               new_session,
                                               boost::asio::placeholders::error));
        }
        
        void tcpserver::handle_accept(tcpsession_ptr new_session, const boost::system::error_code& error)
        {
            if (!error && tcpsessionmgr::ref().insert(new_session->sessionid(), new_session))
            {
                {boost::format fmt("%1%:%2%");
                fmt % new_session->socket().remote_endpoint().address().to_string();
                fmt % new_session->socket().remote_endpoint().port();
                new_session->set_remoteaddr(fmt.str());}
                
                {boost::format fmt("%1%:%2%");
                fmt % new_session->socket().local_endpoint().address().to_string();
                fmt % new_session->socket().local_endpoint().port();
                new_session->set_localaddr(fmt.str());}
                
                if (connectcb_)
                { connectcb_(new_session); }
                
                new_session->start();
            }
            
            start_accept();
        }
        
    }
}


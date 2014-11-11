//
//  tcpclient.cpp
//  console
//
//  Created by huanghaifeng on 14-10-2.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include "tcpclient.h"
#include "tcpsessionmgr.h"

using boost::asio::ip::tcp;

namespace frog
{
    namespace generic
    {
        
        tcpclient::tcpclient(boost::asio::io_service& iosrv)
            : iosrv_(iosrv),
              session_(boost::make_shared<tcpsession>(iosrv, tcpsession_client, parse_pack))
        {
        }
        
        void tcpclient::connect(const char* ip, const char* port)
        {
            session_->set_messagecb(boost::bind(&tcpclient::on_message,this,_1,_2));
            session_->set_closecb(closecb_);
            
            tcp::resolver resolver(iosrv_);
            tcp::resolver::query query(tcp::v4(), ip, port);
            tcp::resolver::iterator iterator = resolver.resolve(query);

            /* it can cause null shared_ptr on centos */
            /*session_->socket().async_connect(*iterator,
                                             boost::bind(&tcpclient::handle_connect, this,
                                                         boost::asio::placeholders::error,
                                                         ++iterator, session_));*/

            /* it can cause asio.misc:3 error code on centos */
            /*boost::asio::async_connect(session_->socket(), iterator,
                                       boost::bind(&tcpclient::handle_connect, 
                                                   this, boost::asio::placeholders::error, 
                                                   ++iterator, session_));*/

            /* it can only work in this way on centos */
            boost::asio::connect(session_->socket(), iterator);
            handle_connect(boost::system::error_code(), ++iterator, session_);
        }
        
        void tcpclient::close()
        {
            session_->socket().close();
            tcpsessionmgr::ref().erase(session_->sessionid());
        }
        
        int tcpclient::on_message(tcpsession_ptr session, decoder* pack)
        {
            if (msgcb_)
            { msgcb_(session, pack); }
            return 0;
        }
        
        void tcpclient::handle_connect(const boost::system::error_code& error,
                            boost::asio::ip::tcp::resolver::iterator it,
                            tcpsession_ptr session)
        {
            if (!error && tcpsessionmgr::ref().insert(session->sessionid(), session))
            {
                {boost::format fmt("%1%:%2%");
                fmt % session->socket().remote_endpoint().address().to_string();
                fmt % session->socket().remote_endpoint().port();
                session->set_remoteaddr(fmt.str());}
                
                {boost::format fmt("%1%:%2%");
                fmt % session->socket().local_endpoint().address().to_string();
                fmt % session->socket().local_endpoint().port();
                session->set_localaddr(fmt.str());}
                
                if (connectcb_)
                { connectcb_(session); }
                
                session_->receive();
            }
            else if (it != tcp::resolver::iterator())
            {
                session_->socket().close();
                session_->socket().async_connect(*it,
                                                 boost::bind(&tcpclient::handle_connect, this,
                                                             boost::asio::placeholders::error,
                                                             ++it, session_));
            }
            else
            {
/*std::cout << error << std::endl;
if(error)
{
std::cout << error.value() << std::endl; 
std::cout << error.category().name() << std::endl; 
}*/
                session_->socket().close();
                if (connectcb_)
                { tcpsession_ptr nptr; connectcb_(nptr); }
                tcpsessionmgr::ref().erase(session_->sessionid());
            }
        }
        
    }
}

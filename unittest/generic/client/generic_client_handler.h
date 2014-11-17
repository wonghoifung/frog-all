//
//  generic_client_handler.h
//  test_client
//
//  Created by huanghaifeng on 14-10-3.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef generic_client_handler_header
#define generic_client_handler_header

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "generic/tcpclient.h"
#include "utils/global_holder.h"

namespace frog
{
    namespace generic
    {
        
        class generic_client_handler : boost::noncopyable
        {
        public:
            void installcb(tcpclient& client)
            {
                client.set_messagecb(boost::bind(&generic_client_handler::on_message,this,_1,_2));
                client.set_closecb(boost::bind(&generic_client_handler::on_close,this,_1));
                client.set_connectcb(boost::bind(&generic_client_handler::on_connect,this,_1));
                client.set_errorcb(boost::bind(&generic_client_handler::on_error,this,_1,_2));
            }
            
            int on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
            {
                std::cout << "cmd:" << pack->command() << std::endl;
                std::cout << pack->read_int() << std::endl;
                std::cout << pack->read_cstring() << std::endl;
                session->close_without_notify();
                return 0;
            }
            
            void on_close(frog::generic::tcpsession_ptr session)
            {
                std::cout << session->localaddr() << " - " << session->remoteaddr() << " closed" << std::endl;
            }
            
            void on_connect(frog::generic::tcpsession_ptr session)
            {
                if (session)
                {
                    std::cout << session->localaddr() << " - " << session->remoteaddr() << " connected" << std::endl;
                    frog::generic::encoder out;
                    out.begin(123);
                    out.write_int(888);
                    out.write_short(777);
                    out.write_cstring("generic_client_handler");
                    out.write_short(666);
                    out.write_int(555);
                    out.end();
                    session->send(&out);
                }
                else
                {
                    std::cout << "cannot connect" << std::endl;
                }
            }
            
            void on_error(frog::generic::tcpsession_ptr session, int errcode)
            {
                std::cout << session->localaddr() << " - "  << session->remoteaddr() << " error:" << errcode << std::endl;
            }
        };
        
    }
}

#endif 

//
//  chat_client_handler.h
//  test_client
//
//  Created by huanghaifeng on 14-11-9.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef chat_client_handler_header
#define chat_client_handler_header

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "generic/tcpclient.h"
#include "utils/global_holder.h"

enum
{
    cmd_user_register = 1,
    cmd_force_quit = 3,
    cmd_server_to_user = 6,
    cmd_no_service_found = 7,
};

enum
{
    cmd_user_broadcast = 1001,
};

const int userid = 1;

namespace frog
{
    namespace chat
    {
        
        class chat_client_handler : boost::noncopyable
        {
        public:
            void installcb(generic::tcpclient& client)
            {
                client.set_messagecb(boost::bind(&chat_client_handler::on_message,this,_1,_2));
                client.set_closecb(boost::bind(&chat_client_handler::on_close,this,_1));
                client.set_connectcb(boost::bind(&chat_client_handler::on_connect,this,_1));
                client.set_errorcb(boost::bind(&chat_client_handler::on_error,this,_1,_2));
            }
            
            int on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
            {
                int cmd = pack->command();
                switch (cmd) {
                    case cmd_user_register:
                    {
                        int userid = pack->read_int();
                        int ret = pack->read_int();
                        if (ret == 0)
                        {
                            std::cout << "login success" << std::endl;
                            generic::encoder op;
                            op.begin(cmd_user_broadcast);
                            op.write_int(userid);
                            op.write_string("helloworld");
                            op.end();
                            session->send(&op);
                        }
                        else
                        {
                            std::cout << "login failure, ret:" << ret << std::endl;
                        }
                        break;
                    }
                        
                    case cmd_force_quit:
                        std::cout << "cmd_force_quit" << std::endl;
                        break;
                        
                    case cmd_server_to_user:
                        std::cout << "cmd_server_to_user" << std::endl;
                        break;
                        
                    case cmd_no_service_found:
                        std::cout << "cmd_no_service_found" << std::endl;
                        break;
                        
                    case cmd_user_broadcast:
                    {
                        int tag = pack->read_int();
                        std::cout << "tag: " << tag << std::endl;
                        int userid = pack->read_int();
                        std::string statement = pack->read_cstring();
                        std::cout << userid << " say: " << statement << std::endl;
                        break;
                    }
                        
                    default:
                        break;
                }
                return 0;
            }
            
            void on_close(frog::generic::tcpsession_ptr session)
            {
                std::cout << session->localaddr() <<" - "<< session->remoteaddr() << " closed" << std::endl;
            }
            
            void on_connect(frog::generic::tcpsession_ptr session)
            {
                if (session)
                {
                    std::cout << session->localaddr() << " - " << session->remoteaddr() << " connected" << std::endl;
                    frog::generic::encoder out;
                    out.begin(cmd_user_register);
                    out.write_int(userid);
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


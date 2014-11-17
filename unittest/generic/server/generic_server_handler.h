//
//  generic_server_handler.h
//  console
//
//  Created by huanghaifeng on 14-10-3.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef generic_server_handler_header
#define generic_server_handler_header

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "generic/tcpserver.h"
#include "generic/linecmd_mgr.h"

namespace frog
{
    namespace generic
    {
        
        class generic_server_handler : public boost::noncopyable
        {
        public:
            generic_server_handler()
            {
                clear_stat();
                linecmd_mgr::ref().addlcmd("stat", boost::bind(&generic_server_handler::lcmd_stat, this, _1, _2));
                linecmd_mgr::ref().addlcmd("clear", boost::bind(&generic_server_handler::lcmd_clear, this, _1, _2));
            }
            
            void lcmd_stat(std::vector<std::string>&,tcpsession_ptr session)
            {
                std::stringstream ss;
                ss << "messsage count : " << msgcount_ << std::endl;
                ss << "connect count : " << conncount_ << std::endl;
                ss << "close count : " << closecount_ << std::endl;
                ss << "timeout count : " << timeoutcount_ << std::endl;
                ss << "error count : " << errorcount_ << std::endl;
                session->send_line(ss.str().c_str(), (int)ss.str().size());
            }
            
            void lcmd_clear(std::vector<std::string>&,tcpsession_ptr)
            {
                clear_stat();
            }
            
            void installcb(tcpserver& s)
            {
                s.set_messagecb(boost::bind(&generic_server_handler::on_message,this,_1,_2));
                s.set_closecb(boost::bind(&generic_server_handler::on_close,this,_1));
                s.set_connectcb(boost::bind(&generic_server_handler::on_connect,this,_1));
                s.set_timeoutcb(boost::bind(&generic_server_handler::on_timeout,this,_1));
                s.set_errorcb(boost::bind(&generic_server_handler::on_error,this,_1,_2));
            }
            
            int on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
            {
                ++msgcount_;
                
                int cmd = pack->command();
                
                std::cout << "(" << session->remoteaddr() << ") cmd:" << cmd << ", "
                    << pack->read_int() << ", "
                    << pack->read_short() << ", "
                    << pack->read_cstring() << ", "
                    << pack->read_short() << ", "
                    << pack->read_int() << std::endl;
                
                frog::generic::encoder op;
                op.begin(321);
                op.write_int(1);
                op.write_cstring("g success");
                op.end();
                session->send(&op);
                
                return 0;
            }
            
            void on_close(frog::generic::tcpsession_ptr session)
            {
                ++closecount_;
                
                std::cout << session->localaddr() << " - " << session->remoteaddr() << " closed" << std::endl;
            }
            
            void on_connect(frog::generic::tcpsession_ptr session)
            {
                ++conncount_;
          
                std::cout << session->localaddr() << " - "  << session->remoteaddr() << " connected" << std::endl;
            }
            
            void on_timeout(frog::generic::tcpsession_ptr session)
            {
                ++timeoutcount_;
                
                std::cout << session->localaddr() << " - "  << session->remoteaddr() << " timeout" << std::endl;
            }
            
            void on_error(frog::generic::tcpsession_ptr session, int errcode)
            {
                ++errorcount_;
                
                std::cout << session->localaddr() << " - "  << session->remoteaddr() << " error:" << errcode << std::endl;
            }
            
        private:
            void clear_stat()
            {
                msgcount_ = 0;
                conncount_ = 0;
                closecount_ = 0;
                timeoutcount_ = 0;
                errorcount_ = 0;
            }
            
        private:
            int msgcount_;
            int conncount_;
            int closecount_;
            int timeoutcount_;
            int errorcount_;
        };
        
    }
}

#endif 

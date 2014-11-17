//
//  console_server_handler.h
//  console
//
//  Created by huanghaifeng on 14-11-16.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef console_server_handler_header
#define console_server_handler_header

#include <map>
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "generic/tcpserver.h"
#include "proxy_client_handler.h"

namespace frog
{
    namespace console
    {
        
        class console_server_handler : public boost::noncopyable
        {
	    typedef boost::shared_ptr<frog::generic::tcpclient> tcpclient_ptr; 
	    typedef boost::shared_ptr<frog::generic::proxy_client_handler> clienthandler_ptr;

	    struct user
	    {
    		int id;
		tcpclient_ptr tcpclient;
		clienthandler_ptr clienthandler;
	    };

        public:
            console_server_handler();
            void lcmd_console_stat(std::vector<std::string>&,generic::tcpsession_ptr session);
            void lcmd_console_clear(std::vector<std::string>&,generic::tcpsession_ptr);

            void lcmd_adduser(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_deluser(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_register(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_connectproxy(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_chatbroadcast(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_sendmail(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_listmail(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_checkmail(std::vector<std::string>&,generic::tcpsession_ptr);
            void lcmd_deletemail(std::vector<std::string>&,generic::tcpsession_ptr);

            void installcb(generic::tcpserver& s);
	    generic::tcpsession_ptr linesession() {return linesession_;}

            void on_close(frog::generic::tcpsession_ptr session);
            void on_connect(frog::generic::tcpsession_ptr session);
            void on_timeout(frog::generic::tcpsession_ptr session);
            void on_error(frog::generic::tcpsession_ptr session, int errcode);
            
        private:
            void clear_stat();
	    user* get_user(int userid);
            
        private:
	    std::map<int,user> users_;
	    generic::tcpsession_ptr linesession_;

            int conncount_;
            int closecount_;
            int timeoutcount_;
            int errorcount_;
        };
        
    }
}

#endif


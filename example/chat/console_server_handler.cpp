//
//  console_server_handler.cpp
//  console
//
//  Created by huanghaifeng on 14-11-9.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "console_server_handler.h"
#include "logger.h"
#include "linecmd_mgr.h"
#include "chat_command.h"
#include "global_holder.h"
#include "proxy_client_handler.h"

namespace frog
{
    namespace console
    {
        
        console_server_handler::console_server_handler()
        {
            clear_stat();
            generic::linecmd_mgr::ref().addlcmd("console_stat",
                                                boost::bind(&console_server_handler::lcmd_console_stat,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("console_clear",
                                                boost::bind(&console_server_handler::lcmd_console_clear,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("serverbroadcast",
                                                boost::bind(&console_server_handler::lcmd_server_broadcast,
                                                            this, _1, _2));
        }
        
        void console_server_handler::lcmd_console_stat(std::vector<std::string>&,
                                                       generic::tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "console connect count : " << conncount_ << std::endl;
            ss << "console close count : " << closecount_ << std::endl;
            ss << "console timeout count : " << timeoutcount_ << std::endl;
            ss << "console error count : " << errorcount_ << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void console_server_handler::lcmd_console_clear(std::vector<std::string>&,
                                                             generic::tcpsession_ptr)
        {
            clear_stat();
        }
        
        void console_server_handler::lcmd_server_broadcast(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(argv.size()!=1)
	    {
		std::stringstream ss;
		ss << "usage: serverbroadcast <CONTENT>" << std::endl;
            	session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }
	    frog::generic::encoder op;
	    op.begin(chat::cmd_server_to_user);
	    op.write_int(0);
	    op.write_int(0);
	    op.write_string(argv[0]);
	    op.end();
	    frog::utils::global_holder<frog::chat::proxy_client_handler>::ref().get().send(&op);
	}

        void console_server_handler::installcb(generic::tcpserver& s)
        {
            s.set_closecb(boost::bind(&console_server_handler::on_close,this,_1));
            s.set_connectcb(boost::bind(&console_server_handler::on_connect,this,_1));
            s.set_timeoutcb(boost::bind(&console_server_handler::on_timeout,this,_1));
            s.set_errorcb(boost::bind(&console_server_handler::on_error,this,_1,_2));
        }
        
        void console_server_handler::on_close(frog::generic::tcpsession_ptr session)
        {
            ++closecount_;
            logstr("console %s - %s closed",session->localaddr().c_str(),session->remoteaddr().c_str());
        }
        
        void console_server_handler::on_connect(frog::generic::tcpsession_ptr session)
        {
            ++conncount_;
            logstr("console %s - %s connected",session->localaddr().c_str(),session->remoteaddr().c_str());
        }
        
        void console_server_handler::on_timeout(frog::generic::tcpsession_ptr session)
        {
            ++timeoutcount_;
            logstr("console %s - %s timeout",session->localaddr().c_str(),session->remoteaddr().c_str());
        }
        
        void console_server_handler::on_error(frog::generic::tcpsession_ptr session, int errcode)
        {
            ++errorcount_;
            logstr("console %s - %s error:%d",
                   session->localaddr().c_str(),
                   session->remoteaddr().c_str(),
                   errcode);
        }
        
        void console_server_handler::clear_stat()
        {
            conncount_ = 0;
            closecount_ = 0;
            timeoutcount_ = 0;
            errorcount_ = 0;
        }
        
    }
}


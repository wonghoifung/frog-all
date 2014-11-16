//
//  console_server_handler.cpp
//  console
//
//  Created by huanghaifeng on 14-11-16.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "console_server_handler.h"
#include "logger.h"
#include "linecmd_mgr.h"
#include "global_holder.h"
#include "string_utils.h"

namespace frog
{
    namespace console
    {
        
        console_server_handler::console_server_handler():tcpclient_(global_io_service.get())
        {
            clear_stat();
	    clienthandler_.set_console(this);
	    clienthandler_.installcb(tcpclient_);

            generic::linecmd_mgr::ref().addlcmd("console_stat",
                                                boost::bind(&console_server_handler::lcmd_console_stat,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("console_clear",
                                                boost::bind(&console_server_handler::lcmd_console_clear,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("setuid",
                                                boost::bind(&console_server_handler::lcmd_setuid,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("register",
                                                boost::bind(&console_server_handler::lcmd_register,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("connectproxy",
                                                boost::bind(&console_server_handler::lcmd_connectproxy,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("chatbroadcast",
                                                boost::bind(&console_server_handler::lcmd_chatbroadcast,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("sendmail",
                                                boost::bind(&console_server_handler::lcmd_sendmail,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("listmail",
                                                boost::bind(&console_server_handler::lcmd_listmail,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("checkmail",
                                                boost::bind(&console_server_handler::lcmd_checkmail,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("deletemail",
                                                boost::bind(&console_server_handler::lcmd_deletemail,
                                                            this, _1, _2));
        }
        
        void console_server_handler::lcmd_console_stat(std::vector<std::string>&,generic::tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "console connect count : " << conncount_ << std::endl;
            ss << "console close count : " << closecount_ << std::endl;
            ss << "console timeout count : " << timeoutcount_ << std::endl;
            ss << "console error count : " << errorcount_ << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void console_server_handler::lcmd_console_clear(std::vector<std::string>&,generic::tcpsession_ptr)
        {
            clear_stat();
        }
        
        void console_server_handler::lcmd_setuid(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    static bool alreadyset = false;
	    if(alreadyset)
     	    {
		std::stringstream ss;
                ss << "cannt set userid again, userid:" << global_user.get().id << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
	    	return;
	    }

	    std::stringstream ss;
            if(argv.size()!=1)
	    {
                ss << "usage: setuid <USERID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    global_user.get().id = frog::utils::string_to_number<int>(argv[0]);

	    ss << "set userid to " << global_user.get().id << std::endl;
	    session->send_line(ss.str().c_str(), (int)ss.str().size());

	    alreadyset = true;
	}

        void console_server_handler::lcmd_register(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(!clienthandler_.is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(clienthandler_.is_registered())
	    {
		std::stringstream ss;
		ss << "already registered" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

            frog::generic::encoder out;
            out.begin(cmd_user_register);
            out.write_int(global_user.get().id);
            out.end();
            clienthandler_.session()->send(&out);
	}

        void console_server_handler::lcmd_connectproxy(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(clienthandler_.is_connected()) 
	    {
		std::stringstream ss;
		ss << "already connected" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }
	    tcpclient_.connect("127.0.0.1", "8888");
	}

        void console_server_handler::lcmd_chatbroadcast(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(!clienthandler_.is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!clienthandler_.is_registered())
	    {
		std::stringstream ss;
		ss << "register first" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(argv.size() != 1)
	    {
		std::stringstream ss;
                ss << "usage: chatbroadcast <CONTENT>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

            generic::encoder op;
            op.begin(cmd_user_broadcast);
            op.write_int(global_user.get().id);
            op.write_string(argv[0]);
            op.end();
            clienthandler_.session()->send(&op);
	}

        void console_server_handler::lcmd_sendmail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(!clienthandler_.is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!clienthandler_.is_registered())
	    {
		std::stringstream ss;
		ss << "register first" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }
	    
	    if(argv.size() != 4)
	    {
		std::stringstream ss;
                ss << "usage: sendmail <TO_USERID> <TITLE> <CONTENT> <ATTACHMENT>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }
 
	    frog::generic::encoder op;
	    op.begin(cmd_user_send_mail);
	    op.write_int(global_user.get().id);
            op.write_int(frog::utils::string_to_number<int>(argv[0]));
	    op.write_string(argv[1]);
	    op.write_string(argv[2]);
	    op.write_string(argv[3]);
            op.end();
            clienthandler_.session()->send(&op);
	}

        void console_server_handler::lcmd_listmail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(!clienthandler_.is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!clienthandler_.is_registered())
	    {
		std::stringstream ss;
		ss << "register first" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    frog::generic::encoder op;
	    op.begin(cmd_user_get_all_mail_brief);
	    op.write_int(global_user.get().id);
	    op.end();
	    clienthandler_.session()->send(&op);
	}

        void console_server_handler::lcmd_checkmail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(!clienthandler_.is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!clienthandler_.is_registered())
	    {
		std::stringstream ss;
		ss << "register first" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(argv.size() != 1)
	    {
		std::stringstream ss;
                ss << "usage: checkmail <MAILID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    frog::generic::encoder op;
	    op.begin(cmd_user_get_mail_detail);
	    op.write_int(global_user.get().id);
	    op.write_int(frog::utils::string_to_number<int>(argv[0]));
	    op.end();
	    clienthandler_.session()->send(&op);
	}

        void console_server_handler::lcmd_deletemail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    if(!clienthandler_.is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!clienthandler_.is_registered())
	    {
		std::stringstream ss;
		ss << "register first" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(argv.size() != 1)
	    {
		std::stringstream ss;
                ss << "usage: deletemail <MAILID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    frog::generic::encoder op;
	    op.begin(cmd_user_delete_mail);
	    op.write_int(global_user.get().id);
	    op.write_int(frog::utils::string_to_number<int>(argv[0]));
	    op.end();
	    clienthandler_.session()->send(&op);
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
	    linesession_ = frog::generic::tcpsession_ptr();
        }
        
        void console_server_handler::on_connect(frog::generic::tcpsession_ptr session)
        {
            ++conncount_;
            logstr("console %s - %s connected",session->localaddr().c_str(),session->remoteaddr().c_str());
	    linesession_ = session;
        }
        
        void console_server_handler::on_timeout(frog::generic::tcpsession_ptr session)
        {
            ++timeoutcount_;
            logstr("console %s - %s timeout",session->localaddr().c_str(),session->remoteaddr().c_str());
	    linesession_ = frog::generic::tcpsession_ptr();
        }
        
        void console_server_handler::on_error(frog::generic::tcpsession_ptr session, int errcode)
        {
            ++errorcount_;
            logstr("console %s - %s error:%d",
                   session->localaddr().c_str(),
                   session->remoteaddr().c_str(),
                   errcode);
	    linesession_ = frog::generic::tcpsession_ptr();
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


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
#include <boost/make_shared.hpp>

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
            generic::linecmd_mgr::ref().addlcmd("adduser",
                                                boost::bind(&console_server_handler::lcmd_adduser,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("deluser",
                                                boost::bind(&console_server_handler::lcmd_deluser,
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
	    ss << "user count : " << users_.size() << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void console_server_handler::lcmd_console_clear(std::vector<std::string>&,generic::tcpsession_ptr)
        {
            clear_stat();
        }
        
        void console_server_handler::lcmd_adduser(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    std::stringstream ss;
            if(argv.size()!=1)
	    {
                ss << "usage: adduser <USERID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);

	    if(users_.find(userid)!=users_.end())
     	    {
                ss << "cannot adduser repeatedly, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
	    	return;
	    }
	    
	    users_[userid].id = userid;
	    users_[userid].tcpclient = boost::make_shared<frog::generic::tcpclient>(boost::ref(global_io_service.get()));
	    users_[userid].clienthandler = boost::make_shared<frog::generic::proxy_client_handler>(userid);
	    users_[userid].clienthandler->set_console(this);
	    users_[userid].clienthandler->installcb(*(users_[userid].tcpclient));
	    
	    ss << "adduser " << userid << " successfully" << std::endl;
	    session->send_line(ss.str().c_str(), (int)ss.str().size());
	}

        void console_server_handler::lcmd_deluser(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
	    std::stringstream ss;
            if(argv.size()!=1)
	    {
                ss << "usage: deluser <USERID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(user)
	    {
		if(user->clienthandler->is_connected())
		{
		    user->tcpclient->close();
		}
		users_.erase(userid);
	    }
	    
	    ss << "deluser " << userid << " successfully" << std::endl;
	    session->send_line(ss.str().c_str(), (int)ss.str().size());
	}

        void console_server_handler::lcmd_register(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
            if(argv.size()!=1)
	    {
	        std::stringstream ss;
                ss << "usage: register <USERID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(!user)
	    {
	 	std::stringstream ss;
		ss << "adduser first, not found userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(user->clienthandler->is_registered())
	    {
		std::stringstream ss;
		ss << "already registered, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

            frog::generic::encoder out;
            out.begin(cmd_user_register);
            out.write_int(userid);
            out.end();
            user->clienthandler->session()->send(&out);
	}

        void console_server_handler::lcmd_connectproxy(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
            if(argv.size()!=1)
	    {
	        std::stringstream ss;
                ss << "usage: connectproxy <USERID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(!user)
	    {
	 	std::stringstream ss;
		ss << "adduser first, not found userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(user->clienthandler->is_connected()) 
	    {
		std::stringstream ss;
		ss << "already connected, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }
	    user->tcpclient->connect("127.0.0.1", "8888");
	}

        void console_server_handler::lcmd_chatbroadcast(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
            if(argv.size()!=2)
	    {
	        std::stringstream ss;
                ss << "usage: chatbroadcast <USERID> <CONTENT>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(!user)
	    {
	 	std::stringstream ss;
		ss << "adduser first, not found userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_registered())
	    {
		std::stringstream ss;
		ss << "register first, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

            generic::encoder op;
            op.begin(cmd_user_broadcast);
            op.write_int(userid);
            op.write_string(argv[1]);
            op.end();
            user->clienthandler->session()->send(&op);
	}

        void console_server_handler::lcmd_sendmail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
            if(argv.size()!=5)
	    {
	        std::stringstream ss;
                ss << "usage: sendmail <USERID> <TO_USERID> <TITLE> <CONTENT> <ATTACHMENT>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(!user)
	    {
	 	std::stringstream ss;
		ss << "adduser first, not found userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_registered())
	    {
		std::stringstream ss;
		ss << "register first, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }
	    
	    frog::generic::encoder op;
	    op.begin(cmd_user_send_mail);
	    op.write_int(userid);
            op.write_int(frog::utils::string_to_number<int>(argv[1]));
	    op.write_string(argv[2]);
	    op.write_string(argv[3]);
	    op.write_string(argv[4]);
            op.end();
            user->clienthandler->session()->send(&op);
	}

        void console_server_handler::lcmd_listmail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
            if(argv.size()!=1)
	    {
	        std::stringstream ss;
                ss << "usage: listmail <USERID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(!user)
	    {
	 	std::stringstream ss;
		ss << "adduser first, not found userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_registered())
	    {
		std::stringstream ss;
		ss << "register first, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    frog::generic::encoder op;
	    op.begin(cmd_user_get_all_mail_brief);
	    op.write_int(userid);
	    op.end();
	    user->clienthandler->session()->send(&op);
	}

        void console_server_handler::lcmd_checkmail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
            if(argv.size()!=2)
	    {
	        std::stringstream ss;
                ss << "usage: checkmail <USERID> <MAILID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(!user)
	    {
	 	std::stringstream ss;
		ss << "adduser first, not found userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_registered())
	    {
		std::stringstream ss;
		ss << "register first, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    frog::generic::encoder op;
	    op.begin(cmd_user_get_mail_detail);
	    op.write_int(userid);
	    op.write_int(frog::utils::string_to_number<int>(argv[1]));
	    op.end();
	    user->clienthandler->session()->send(&op);
	}

        void console_server_handler::lcmd_deletemail(std::vector<std::string>& argv,generic::tcpsession_ptr session)
	{
            if(argv.size()!=2)
	    {
	        std::stringstream ss;
                ss << "usage: deletemail <USERID> <MAILID>" << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    int userid = frog::utils::string_to_number<int>(argv[0]);
	    user* user = get_user(userid);
	    if(!user)
	    {
	 	std::stringstream ss;
		ss << "adduser first, not found userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_connected()) 
	    {
		std::stringstream ss;
		ss << "tcpclient not connected, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    if(!user->clienthandler->is_registered())
	    {
		std::stringstream ss;
		ss << "register first, userid:" << userid << std::endl;
		session->send_line(ss.str().c_str(), (int)ss.str().size());
		return;
	    }

	    frog::generic::encoder op;
	    op.begin(cmd_user_delete_mail);
	    op.write_int(userid);
	    op.write_int(frog::utils::string_to_number<int>(argv[1]));
	    op.end();
	    user->clienthandler->session()->send(&op);
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
        
	console_server_handler::user* console_server_handler::get_user(int userid)
	{
	    std::map<int,user>::iterator it = users_.find(userid);
	    if(it == users_.end()) return NULL;
	    return &(it->second);
	}

    }
}


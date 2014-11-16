//
//  proxy_client_handler.cpp
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "proxy_client_handler.h"
#include "mail_command.h"
#include "logger.h"
#include "config.h"
#include "generic/linecmd_mgr.h"

namespace frog
{
    namespace mail
    {
        
        proxy_client_handler::proxy_client_handler()
        {
            generic::linecmd_mgr::ref().addlcmd("chat_stat",
                                                boost::bind(&proxy_client_handler::lcmd_mail_stat,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("chat_clear",
                                                boost::bind(&proxy_client_handler::lcmd_mail_clear,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("chat_stat_more",
                                                boost::bind(&mail_manager::lcmd_mail_stat_more,
                                                            &mgr_, _1, _2));
        }
        
        void proxy_client_handler::lcmd_mail_stat(std::vector<std::string>&,
                                                  generic::tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "reconnect count : " << stat_.reconn_count_ << std::endl;
            ss << "receive message count : " << stat_.receive_msgcount_ << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void proxy_client_handler::lcmd_mail_clear(std::vector<std::string>&,
                                                   generic::tcpsession_ptr session)
        {
            stat_.clear();
        }
        
        void proxy_client_handler::installcb(generic::tcpclient& client)
        {
            client.set_messagecb(boost::bind(&proxy_client_handler::on_message,this,_1,_2));
            client.set_closecb(boost::bind(&proxy_client_handler::on_close,this,_1));
            client.set_connectcb(boost::bind(&proxy_client_handler::on_connect,this,_1));
            client.set_errorcb(boost::bind(&proxy_client_handler::on_error,this,_1,_2));
        }
        
        int proxy_client_handler::on_message(frog::generic::tcpsession_ptr session,
                                             frog::generic::decoder* pack)
        {
	    if(!session)return -1;
	    if(pack == NULL)return -1;
            ++stat_.receive_msgcount_;
            mgr_.dispatch(session, pack);
            return 0;
        }
        
        void proxy_client_handler::on_close(frog::generic::tcpsession_ptr session)
        {
            logstr("%s - %s closed", session->localaddr().c_str(), session->remoteaddr().c_str());
        }
        
        void proxy_client_handler::on_connect(frog::generic::tcpsession_ptr session)
        {
            if (session)
            {
                logstr("%s - %s connected", session->localaddr().c_str(), session->remoteaddr().c_str());
                frog::generic::encoder out;
                out.begin(cmd_server_register);
                out.write_int(config::ref().get_servertype());
                out.end();
                session->send(&out);
            }
            else
            {
                logstr("cannot connect to proxy");
            }
        }
        
        void proxy_client_handler::on_error(frog::generic::tcpsession_ptr session, int errcode)
        {
            logstr("%s - %s error:%d", session->localaddr().c_str(), session->remoteaddr().c_str(), errcode);
        }
        
    }
}

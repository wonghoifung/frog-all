//
//  proxy_server_handler.cpp
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "proxy_server_handler.h"
#include "config.h"
#include "logger.h"

namespace frog
{
    namespace proxy
    {
        
        proxy_server_handler::proxy_server_handler():starttime_(time(NULL))
        {
            clear_stat();
            generic::linecmd_mgr::ref().addlcmd("stat",
                                                boost::bind(&proxy_server_handler::lcmd_stat,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("clear",
                                                boost::bind(&proxy_server_handler::lcmd_clear,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("reload",
                                                boost::bind(&proxy_server_handler::lcmd_reload,
                                                            this, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("dispatch_stat",
                                                boost::bind(&dispatcher::lcmd_dispatch_stat,
                                                            &dispatcher_, _1, _2));
            generic::linecmd_mgr::ref().addlcmd("dispatch_stat_clear",
                                                boost::bind(&dispatcher::lcmd_dispatch_stat_clear,
                                                            &dispatcher_, _1, _2));
        }
        
        void proxy_server_handler::lcmd_stat(std::vector<std::string>&,generic::tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "start time : " << utils::get_string_time(starttime_) << std::endl;
            ss << "messsage count : " << msgcount_ << std::endl;
            ss << "connect count : " << conncount_ << std::endl;
            ss << "close count : " << closecount_ << std::endl;
            ss << "timeout count : " << timeoutcount_ << std::endl;
            ss << "error count : " << errorcount_ << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void proxy_server_handler::lcmd_clear(std::vector<std::string>&,generic::tcpsession_ptr)
        {
            clear_stat();
        }
        
        void proxy_server_handler::lcmd_reload(std::vector<std::string>& paras,generic::tcpsession_ptr)
        {
            std::string cfgfile = "./frog_cfg.xml";
            if (paras.size())
            {
                cfgfile = paras[0];
            }
            
            if (!config::ref().init(cfgfile))
            {
                logstr("reload config error");
                return;
            }
            logstr("reload config success");
            config::ref().dump();
        }
        
        void proxy_server_handler::installcb(generic::tcpserver& s)
        {
            s.set_messagecb(boost::bind(&proxy_server_handler::on_message,this,_1,_2));
            s.set_closecb(boost::bind(&proxy_server_handler::on_close,this,_1));
            s.set_connectcb(boost::bind(&proxy_server_handler::on_connect,this,_1));
            s.set_timeoutcb(boost::bind(&proxy_server_handler::on_timeout,this,_1));
            s.set_errorcb(boost::bind(&proxy_server_handler::on_error,this,_1,_2));
        }
        
        int proxy_server_handler::on_message(frog::generic::tcpsession_ptr session,
                                             frog::generic::decoder* pack)
        {
            ++msgcount_;
            dispatcher_.dispatch(session, pack);
            return 0;
        }
        
        void proxy_server_handler::on_close(frog::generic::tcpsession_ptr session)
        {
            ++closecount_;
            logstr("%s - %s closed",session->localaddr().c_str(),session->remoteaddr().c_str());
            dispatcher_.handle_disconnect(session);
        }
        
        void proxy_server_handler::on_connect(frog::generic::tcpsession_ptr session)
        {
            ++conncount_;
            logstr("%s - %s connected",session->localaddr().c_str(),session->remoteaddr().c_str());
            int ret = session->set_sndbuf_size(1024*64);
            if (0 != ret)
            {
                logstr("peer(%s), set_sndbuf_size failure, err:%d", session->remoteaddr().c_str(), ret);
            }
            ret = session->set_rcvbuf_size(1024*32);
            if (0 != ret)
            {
                logstr("peer(%s), set_rcvbuf_size failure, err:%d", session->remoteaddr().c_str(), ret);
            }
        }
        
        void proxy_server_handler::on_timeout(frog::generic::tcpsession_ptr session)
        {
            ++timeoutcount_;
            logstr("%s - %s timeout",session->localaddr().c_str(),session->remoteaddr().c_str());
            dispatcher_.handle_disconnect(session);
        }
        
        void proxy_server_handler::on_error(frog::generic::tcpsession_ptr session, int errcode)
        {
            ++errorcount_;
            logstr("%s - %s error:%d",session->localaddr().c_str(),session->remoteaddr().c_str(),errcode);
            dispatcher_.handle_disconnect(session);
        }
        
        void proxy_server_handler::clear_stat()
        {
            msgcount_ = 0;
            conncount_ = 0;
            closecount_ = 0;
            timeoutcount_ = 0;
            errorcount_ = 0;
        }
        
    }
}

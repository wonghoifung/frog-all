//
//  tcpsessionmgr.cpp
//  console
//
//  Created by huanghaifeng on 14-10-3.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "tcpsessionmgr.h"
#include "encoder.h"
#include "linecmd_mgr.h"

namespace frog
{
    namespace generic
    {
        
        tcpsessionmgr::tcpsessionmgr()
            : sessiontimer_(utils::global_holder<boost::asio::io_service>::ref().get())
        {
            linecmd_mgr::ref().addlcmd("scount", boost::bind(&tcpsessionmgr::lcmd_scount, this, _1, _2));
            linecmd_mgr::ref().addlcmd("sconn", boost::bind(&tcpsessionmgr::lcmd_sconn, this, _1, _2));
        }
        
        void tcpsessionmgr::on_timeout()
        {
            std::map<int, tcpsession_ptr>::iterator it(sessions_.begin());
            while (it != sessions_.end())
            {
                if (it->second->sessiontype() == tcpsession_client)
                {
                    if (!it->second->socket().is_open())
                    {
                        if (errorcb_)
                        { errorcb_(it->second, 0); }
                        it->second->close_without_notify();
                        //it = sessions_.erase(it);
                        sessions_.erase(it++); /* gcc version 4.1.2 20080704 (Red Hat 4.1.2-54) */
                        continue;
                    }
                    else /* heart beat */
                    {
                        encoder op;
                        op.begin(cmd_heart_beat);
                        op.end();
                        it->second->send(&op);
                        it->second->set_active();
                    }
                }
                else if (it->second->sessiontype() == tcpsession_server)
                {
                    //std::cout<<"check session"<<std::endl;
                    if (!it->second->socket().is_open())
                    {
                        if (errorcb_)
                        { errorcb_(it->second, 0); }
                        it->second->close_without_notify();
                        //it = sessions_.erase(it);
                        sessions_.erase(it++);
                        continue;
                    }
                    else if (it->second->timeout())
                    {
                        if (timeoutcb_)
                        { timeoutcb_(it->second); }
                        it->second->close_without_notify();
                        //it = sessions_.erase(it);
                        sessions_.erase(it++);
                        continue;
                    }
                }
                ++it;
            }
            
            /* there're still sessions */
            if (sessions_.size() > 0)
            { start_timer(); }
        }
        
        void tcpsessionmgr::start_timer()
        {
            sessiontimer_.expires_from_now(boost::posix_time::seconds(5));
            sessiontimer_.async_wait(boost::bind(&tcpsessionmgr::on_timeout, this));
        }
        
        bool tcpsessionmgr::insert(int sid, tcpsession_ptr session)
        {
            if ( sessions_.insert(std::make_pair(sid, session)).second )
            {
                /* the first session come in, start timer */
                if (sessions_.size() == 1)
                { start_timer(); }
                return true;
            }
            return false;
        }
        
        bool tcpsessionmgr::erase(int sid)
        {
            sessions_.erase(sid);
            /* no session left, stop timer */
            if (sessions_.empty())
            { sessiontimer_.cancel(); }
            return true;
        }
        
        void tcpsessionmgr::lcmd_scount(std::vector<std::string>&,tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "session count: " << sessions_.size() << "\n";
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void tcpsessionmgr::lcmd_sconn(std::vector<std::string>&,tcpsession_ptr session)
        {
            std::stringstream ss;
            std::map<int, tcpsession_ptr>::iterator beg(sessions_.begin()), end(sessions_.end());
            for (; beg != end; ++beg)
            {
                ss << "sid " << beg->first << ": "
                   << beg->second->localaddr() << " - "
                   << beg->second->remoteaddr() << "\n";
            }
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
    }
}

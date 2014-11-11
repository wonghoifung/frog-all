//
//  tcpsessionmgr.h
//  console
//
//  Created by huanghaifeng on 14-10-3.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef tcpsessionmgr_header
#define tcpsessionmgr_header

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <map>
#include "tcpsession.h"
#include "../utils/global_holder.h"

namespace frog
{
    namespace generic
    {
        
        class tcpsessionmgr : public boost::noncopyable
        {
        private:
            tcpsessionmgr();
            ~tcpsessionmgr(){}
            void on_timeout();
            void start_timer();
            
        public:
            static tcpsessionmgr& ref()
            {
                static tcpsessionmgr instance;
                return instance;
            }
            
            bool find(int sid)
            {
                return (sessions_.find(sid) != sessions_.end());
            }
            
            bool insert(int sid, tcpsession_ptr session);
            bool erase(int sid);
            
            void set_errorcb(error_callback cb) { errorcb_ = cb; }
            void set_timeoutcb(timeout_callback cb) { timeoutcb_ = cb; }
            
            /* for line command */
            void lcmd_scount(std::vector<std::string>&,tcpsession_ptr);
            void lcmd_sconn(std::vector<std::string>&,tcpsession_ptr);
            
        private:
            std::map<int, tcpsession_ptr> sessions_;
            boost::asio::deadline_timer sessiontimer_;
            error_callback errorcb_;
            timeout_callback timeoutcb_;
        };
        
    }
}

#endif

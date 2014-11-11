//
//  chat_manager.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef chat_manager_header
#define chat_manager_header

#include <boost/noncopyable.hpp>
#include "generic/tcpserver.h"
#include "user.h"

namespace frog
{
    namespace chat
    {

        typedef int userid;
        typedef boost::shared_ptr<user> user_ptr;
        typedef std::map<userid, user_ptr> user_map;
        
        class chat_manager : public boost::noncopyable
        {
        public:
            chat_manager();
            ~chat_manager();
            void lcmd_chat_stat_more(std::vector<std::string>&,generic::tcpsession_ptr session);
            void dispatch(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            
        private:
            void handle_user_login(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            void handle_user_logout(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            void handle_user_broadcast(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            
        private:
            user_map users_;
        };
        
    }
}

#endif 


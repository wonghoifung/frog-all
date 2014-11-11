//
//  chat_manager.cpp
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "chat_manager.h"
#include "logger.h"
#include "chat_command.h"
#include "user.h"
#include "config.h"
#include <boost/make_shared.hpp>

namespace frog
{
    namespace chat
    {
        
        chat_manager::chat_manager()
        {
            
        }
        
        chat_manager::~chat_manager()
        {
            
        }
        
        void chat_manager::lcmd_chat_stat_more(std::vector<std::string>&,generic::tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "user count : " << users_.size() << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void chat_manager::dispatch(frog::generic::tcpsession_ptr session,
                                    frog::generic::decoder* pack)
        {
            int cmd = pack->command();
            switch (cmd) {
                case cmd_broadcast_user_login:
                    handle_user_login(session, pack);
                    break;
                    
                case cmd_broadcast_user_logout:
                    handle_user_logout(session, pack);
                    break;
                    
                case cmd_user_broadcast:
                    handle_user_broadcast(session, pack);
                    
                default:
                    break;
            }
        }
        
        void chat_manager::handle_user_login(frog::generic::tcpsession_ptr session,
                                             frog::generic::decoder* pack)
        {
            int userid = pack->read_int();
            
            user_ptr up = boost::make_shared<user>();
            up->set_id(userid);
            users_[userid] = up;
            
            generic::encoder op;
            op.begin(cmd_user_register);
            op.write_int(userid);
            op.write_int(0); //success
            op.end();
            session->send(&op);
        }
        
        void chat_manager::handle_user_logout(frog::generic::tcpsession_ptr session,
                                              frog::generic::decoder* pack)
        {
            int userid = pack->read_int();
            users_.erase(userid);
        }
        
        void chat_manager::handle_user_broadcast(frog::generic::tcpsession_ptr session,
                                                 frog::generic::decoder* pack)
        {
            int userid = pack->read_int();
            std::string statement = pack->read_cstring();
            
            generic::encoder op;
            op.begin(cmd_user_broadcast);
            op.write_int(0);
            op.write_int(userid);
            op.write_string(statement);
            op.end();
            session->send(&op);
        }
        
    }
}


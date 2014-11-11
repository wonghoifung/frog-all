//
//  dispatcher.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef dispatcher_header
#define dispatcher_header

#include <boost/noncopyable.hpp>
#include "generic/tcpserver.h"

namespace frog
{
    namespace proxy
    {
        
        typedef int userid;
        typedef int serverid;
        typedef int socketid;
        typedef std::map<userid, frog::generic::tcpsession_ptr> user_session_map;
        typedef std::map<serverid, frog::generic::tcpsession_ptr> server_session_map;
        typedef std::map<socketid, frog::generic::tcpsession_ptr> common_session_map;
        
        struct dispatcher_stat
        {
            int user_register_count;
            int server_register_count;
            int user_to_server_request_count;
            int server_to_user_response_count;
            int server_to_server_request_count;
            int server_to_server_response_count;
            int server_to_user_count;
            dispatcher_stat() {clear();}
            void clear()
            {
                user_register_count = 0;
                server_register_count = 0;
                user_to_server_request_count = 0;
                server_to_user_response_count = 0;
                server_to_server_request_count = 0;
                server_to_server_response_count = 0;
                server_to_user_count = 0;
            }
        };
        
        class dispatcher : public boost::noncopyable
        {
        public:
            dispatcher();
            ~dispatcher();
            void dispatch(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            void handle_disconnect(frog::generic::tcpsession_ptr session);
            
        private:
            void handle_user_register(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            void handle_server_register(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            
            void handle_user_to_server_request(frog::generic::tcpsession_ptr session,
                                               frog::generic::decoder* pack);
            void handle_server_to_user_response(frog::generic::tcpsession_ptr session,
                                                frog::generic::decoder* pack);
           
            void handle_common_server_to_server_request(frog::generic::tcpsession_ptr session,
                                                        frog::generic::decoder* pack);
            void handle_common_server_to_server_response(frog::generic::tcpsession_ptr session,
                                                         frog::generic::decoder* pack);
            void handle_common_server_to_user(frog::generic::tcpsession_ptr session,
                                              frog::generic::decoder* pack);
            
            void send_to_all_users(generic::encoder* pack);
            void send_to_user(int userid, generic::encoder* pack);
            
            void service_not_found(frog::generic::tcpsession_ptr session);
            
            void tell_servers_user_login(int userid);
            void tell_servers_user_logout(int userid);
            
        public:
            void lcmd_dispatch_stat(std::vector<std::string>&,generic::tcpsession_ptr session);
            void lcmd_dispatch_stat_clear(std::vector<std::string>&,generic::tcpsession_ptr session);
            
        private:
            user_session_map users_;
            server_session_map servers_;
            /* mainly used between server and server */
            common_session_map common_sessions_;
            dispatcher_stat stat_;
        };
        
    }
}

#endif 


//
//  dispatcher.cpp
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "dispatcher.h"
#include "logger.h"
#include "proxy_command.h"
#include "peer.h"
#include "config.h"
#include <boost/make_shared.hpp>

enum force_quit_reason
{
    force_quit_repeat_login = 0,
};

namespace frog
{
    namespace proxy
    {
        
        dispatcher::dispatcher()
        {
            
        }
        
        dispatcher::~dispatcher()
        {
            
        }
        
        void dispatcher::dispatch(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
        {
            int cmd = pack->command();
            
            logstr("from:%s cmd:%d", session->remoteaddr().c_str(), cmd);
            
            boost::shared_ptr<peer> pr = boost::static_pointer_cast<peer>(session->tag());
            
            /* common message from server */
            switch (cmd)
            {
                case cmd_server_to_server_request:
                    ++stat_.server_to_server_request_count;
                    handle_common_server_to_server_request(session, pack);
                    return;
                    
                case cmd_server_to_server_response:
                    ++stat_.server_to_server_response_count;
                    handle_common_server_to_server_response(session, pack);
                    return;
                    
                case cmd_server_to_user:
                    ++stat_.server_to_user_count;
                    handle_common_server_to_user(session, pack);
                    return;
                    
                default:
                    break;
            }
            
            if (!pr)
            {
                /* this is a new session, register */
                switch (cmd)
                {
                    case cmd_user_register:
                        ++stat_.user_register_count;
                        handle_user_register(session, pack);
                        break;
                        
                    case cmd_server_register:
                        ++stat_.server_register_count;
                        handle_server_register(session, pack);
                        break;
                        
                    default:
                        logstr("register, unknown command:%d", cmd);
                        break;
                }
                return;
            }
            
            /* handle messages between user and server */
            switch (pr->get_peertype())
            {
                case peer_user:
                    ++stat_.user_to_server_request_count;
                    handle_user_to_server_request(session, pack);
                    break;
                    
                case peer_chat:
                case peer_mail:
                    ++stat_.server_to_user_response_count;
                    handle_server_to_user_response(session, pack);
                    break;
                    
                default:
                    logstr("dispatch by peer, unknown peertype:%d, peerid:%d, socketid:%d",
                           pr->get_peertype(), pr->get_id(), session->get_native_handle());
                    break;
            }
        }
        
        void dispatcher::handle_disconnect(frog::generic::tcpsession_ptr session)
        {
            common_sessions_.erase(session->get_native_handle());
            boost::shared_ptr<peer> pr = boost::static_pointer_cast<peer>(session->tag());
            if (pr)
            {
                switch (pr->get_peertype())
                {
                    case peer_user:
                    {
                        users_.erase(pr->get_id());
                        tell_servers_user_logout(pr->get_id());
                        break;
                    }
                        
                    default:
                    {
                        servers_.erase(pr->get_peertype()); /* type equal to id now */
                        logstr("server type:%d, id:%d, disconnect", pr->get_peertype(), pr->get_id());
                        break;
                    }
                }
                session->set_tag(generic::void_ptr());
            }
        }
        
        void dispatcher::handle_user_register(frog::generic::tcpsession_ptr session,
                                              frog::generic::decoder* pack)
        {
            int userid = pack->read_int();

            user_session_map::iterator it = users_.find(userid);
            if (it != users_.end())
            {
                generic::encoder op;
                op.begin(cmd_force_quit);
                op.write_int(force_quit_repeat_login);
                op.end();
                it->second->send(&op);
                
                boost::shared_ptr<peer> pr = boost::static_pointer_cast<peer>(it->second->tag());
                
                tell_servers_user_logout(pr->get_id());
                
                it->second->set_tag(generic::void_ptr());
                it->second->close_without_notify();
                users_.erase(it);
            }

            /* new peer */
            boost::shared_ptr<peer> pr = boost::make_shared<peer>();

            if (pr)
            {
                pr->set_peertype(peer_user);
                pr->set_id(userid);
                
                session->set_tag(pr);
                
                users_[userid] = session;
                
                tell_servers_user_login(userid);
            }
            else
            {
                logstr("user:%d login failure", userid);
            }
        }
        
        void dispatcher::handle_server_register(frog::generic::tcpsession_ptr session,
                                                frog::generic::decoder* pack)
        {
            int servertype = pack->read_int();
            int serverid = pack->read_int(); /* not use now */

            server_session_map::iterator it = servers_.find(serverid);
            if (it != servers_.end())
            {
                /* maybe no need to do this */
                generic::encoder op;
                op.begin(cmd_force_quit);
                op.write_int(force_quit_repeat_login);
                op.end();
                it->second->send(&op);
                
                boost::shared_ptr<peer> pr = boost::static_pointer_cast<peer>(it->second->tag());
                assert(pr->get_peertype() == servertype);
                
                it->second->set_tag(generic::void_ptr());
                it->second->close_without_notify();
                servers_.erase(it);
            }
            
            int ret = session->set_sndbuf_size(1024*128);
            if (0 != ret)
            {
                logstr("server type:%d, id:%d, set_sndbuf_size failure, err:%d", servertype, serverid, ret);
            }
            
            ret = session->set_rcvbuf_size(1024*128);
            if (0 != ret)
            {
                logstr("server type:%d, id:%d, set_rcvbuf_size failure, err:%d", servertype, serverid, ret);
            }
            
            /* new peer */
            boost::shared_ptr<peer> pr = boost::make_shared<peer>();
            if (pr)
            {
                pr->set_peertype((peer_type)servertype);
                pr->set_id(serverid);
                
                session->set_tag(pr);
                
                //servers_[serverid] = session;
                servers_[servertype] = session; /* servertype equals to serverid now */
            }
            else
            {
                logstr("server type:%d, id:%d login failure", servertype, serverid);
            }
        }
        
        void dispatcher::handle_user_to_server_request(frog::generic::tcpsession_ptr session,
                                                       frog::generic::decoder* pack)
        {
            int cmd = pack->command();
            int servertype(-1);
            
            /* check service */
            std::map<int, command_range>& cmdrng = config::ref().get_cmdrange();
            std::map<int, command_range>::iterator beg(cmdrng.begin()),end(cmdrng.end());
            for (; beg!=end; ++beg)
            {
                if (beg->second.min <= cmd && beg->second.max >= cmd)
                {
                    servertype = beg->first;
                    break;
                }
            }
            
            if (-1 == servertype)
            {
                logstr("user cmd:%d is not in any available range", cmd);
                return;
            }
            
            server_session_map::iterator it = servers_.find(servertype);
            if (it != servers_.end())
            {
                int userid = pack->read_int();
                boost::shared_ptr<peer> pr = boost::static_pointer_cast<peer>(session->tag());
                if (pr->get_id() != userid)
                {
                    logstr("userid:%d, peerid:%d, not equal", userid, pr->get_id());
                    return;
                }
                
                generic::encoder op;
                op.copy(pack->buffer(), pack->size());
                it->second->send(&op);
            }
            else
            {
                logstr("servertype:%d not available", servertype);
                service_not_found(session);
            }
        }

        void dispatcher::handle_server_to_user_response(frog::generic::tcpsession_ptr session,
                                                        frog::generic::decoder* pack)
        {
            generic::encoder op;
            op.copy(pack->buffer(), pack->size());
            int userid = pack->read_int();
            if (0 == userid)
            {
                send_to_all_users(&op);
            }
            else
            {
                send_to_user(userid, &op);
            }
        }
        
        /* common request from server to server */
        void dispatcher::handle_common_server_to_server_request(frog::generic::tcpsession_ptr session,
                                                                frog::generic::decoder* pack)
        {
            if (common_sessions_.find(session->get_native_handle()) == common_sessions_.end())
            {
                common_sessions_[session->get_native_handle()] = session;
            }
            
            int servertype = pack->read_int_and_repack();
            server_session_map::iterator it = servers_.find(servertype);
            if (it != servers_.end())
            {
                generic::encoder op;
                op.copy(pack->buffer(), pack->size());
                op.write_front_int(session->get_native_handle());
                it->second->send(&op);
            }
            else
            {
                logstr("find no servertype:%d", servertype);
            }
        }
        
        /* common response from server to server */
        void dispatcher::handle_common_server_to_server_response(frog::generic::tcpsession_ptr session,
                                                                 frog::generic::decoder* pack)
        {
            int socketid = pack->read_int_and_repack();
            common_session_map::iterator it = common_sessions_.find(socketid);
            if (it != common_sessions_.end())
            {
                generic::encoder op;
                op.copy(pack->buffer(), pack->size());
                it->second->send(&op);
            }
            else
            {
                logstr("find no socketid:%d", socketid);
            }
        }
        
        /* common message from server to user */
        void dispatcher::handle_common_server_to_user(frog::generic::tcpsession_ptr session,
                                                      frog::generic::decoder* pack)
        {
#if 0
            char tmp[generic::c_buffer_size] = {0};
            int size = pack->read_binary(tmp, generic::c_buffer_size);
            assert(size >= generic::c_header_size);
#endif
            //int userid = pack->read_int_and_repack();
            int userid = pack->read_int();
            generic::encoder op;
            op.copy(pack->buffer(), pack->size());
            if (0 == userid)
            {
                send_to_all_users(&op);
            }
            else
            {
                send_to_user(userid, &op);
            }
        }
        
        void dispatcher::send_to_all_users(generic::encoder* pack)
        {
            user_session_map::iterator beg(users_.begin()),end(users_.end());
            for (; beg!=end; ++beg)
            {
                beg->second->send(pack);
            }
        }
        
        void dispatcher::send_to_user(int userid, generic::encoder* pack)
        {
            user_session_map::iterator it = users_.find(userid);
            if (it != users_.end())
            {
                it->second->send(pack);
            }
        }
        
        void dispatcher::service_not_found(frog::generic::tcpsession_ptr session)
        {
            generic::encoder op;
            op.begin(cmd_no_service_found);
            op.end();
            session->send(&op);
        }
        
        void dispatcher::tell_servers_user_login(int userid)
        {
            generic::encoder op;
            op.begin(cmd_broadcast_user_login);
            op.write_int(userid);
            op.end();
            server_session_map::iterator beg(servers_.begin()),end(servers_.end());
            for (; beg!=end; ++beg)
            {
                beg->second->send(&op);
            }
        }
        
        void dispatcher::tell_servers_user_logout(int userid)
        {
            generic::encoder op;
            op.begin(cmd_broadcast_user_logout);
            op.write_int(userid);
            op.end();
            server_session_map::iterator beg(servers_.begin()),end(servers_.end());
            for (; beg!=end; ++beg)
            {
                beg->second->send(&op);
            }
        }

        void dispatcher::lcmd_dispatch_stat(std::vector<std::string>&,generic::tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "user_register_count: " << stat_.user_register_count << std::endl;
            ss << "server_register_count: " << stat_.server_register_count << std::endl;
            ss << "user_to_server_request_count: " << stat_.user_to_server_request_count << std::endl;
            ss << "server_to_user_response_count: " << stat_.server_to_user_response_count << std::endl;
            ss << "server_to_server_request_count: " << stat_.server_to_server_request_count << std::endl;
            ss << "server_to_server_response_count: " << stat_.server_to_server_response_count << std::endl;
            ss << "server_to_user_count: " << stat_.server_to_user_count << std::endl;
            ss << "actual users count: " << users_.size() << std::endl;
            ss << "actual servers count: " << servers_.size() << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void dispatcher::lcmd_dispatch_stat_clear(std::vector<std::string>&,generic::tcpsession_ptr session)
        {
            stat_.clear();
        }
        
    }
}


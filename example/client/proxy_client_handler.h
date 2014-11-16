//
//  proxy_client_handler.h
//  test_client
//
//  Created by huanghaifeng on 14-10-16.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef proxy_client_handler_header
#define proxy_client_handler_header

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "generic/tcpclient.h"
#include "utils/global_holder.h"
#include "client_command.h"
 
namespace frog
{
    
    namespace console 
    {
	class console_server_handler;
    }

    namespace generic
    {
        
        class proxy_client_handler : boost::noncopyable
        {
        public:
	    proxy_client_handler();
	    void set_console(console::console_server_handler* con) {console_ = con;}
	    bool is_connected() {return connected_;}
	    frog::generic::tcpsession_ptr session() {assert(session_); return session_;}
	    bool is_registered() {return registered_;}

            void installcb(tcpclient& client);

            int on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            void on_close(frog::generic::tcpsession_ptr session);
            void on_connect(frog::generic::tcpsession_ptr session);
            void on_error(frog::generic::tcpsession_ptr session, int errcode);

	private:
	    void handle_register(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    void handle_force_quit(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    void handle_server_to_user(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    void handle_no_service_found(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    /* chat */
	    void handle_user_broadcast(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    /* mail */
	    void handle_get_all_mail_brief(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    void handle_send_mail(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    void handle_delete_mail(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    void handle_get_mail_detail(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
	    void handle_mail_alert(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);

	private:
	    void clear_connect_state();

	private:
	    bool connected_;
	    frog::generic::tcpsession_ptr session_;
	    bool registered_;
	    console::console_server_handler* console_;
        };
        
    }
}

#endif
 

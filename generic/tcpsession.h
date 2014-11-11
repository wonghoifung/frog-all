//
//  tcpsession.h
//  console
//
//  Created by huanghaifeng on 14-10-2.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef tcpsession_header
#define tcpsession_header

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp> 
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include "packparser.h"
#include "encoder.h"
#include "command.h"
#include "lineparser.h"

namespace frog
{
    namespace generic
    {
        
        class tcpsession;
        typedef boost::shared_ptr<void> void_ptr;
        typedef boost::shared_ptr<tcpsession> tcpsession_ptr;
        typedef boost::function<int(tcpsession_ptr,decoder*)> message_callback;
        typedef boost::function<void(tcpsession_ptr)> close_callback;
        typedef boost::function<void(tcpsession_ptr)> connect_callback;
        typedef boost::function<void(tcpsession_ptr)> timeout_callback;
        typedef boost::function<void(tcpsession_ptr, int)> error_callback;
        enum tcpsession_type { tcpsession_client=0, tcpsession_server };
        enum parser_type { parse_pack=0, parse_line };
        
        class tcpsession :
            public boost::enable_shared_from_this<tcpsession>,
            public boost::noncopyable
        {
        public:
#if _WIN32
			tcpsession(boost::asio::io_service&,tcpsession_type,parser_type);
#else
            tcpsession(const boost::asio::io_service&,const tcpsession_type,const parser_type);
#endif
            ~tcpsession();
            boost::asio::ip::tcp::socket& socket();
            void start();
            
            void set_messagecb(message_callback cb) { messagecb_ = cb; }
            void set_closecb(close_callback cb) { closecb_ = cb; }
            
            void set_sessionid(int sid) { sessionid_ = sid; }
            int sessionid() { return sessionid_; }
            
            int get_native_handle() {return static_cast<int>(socket_.native_handle());}
            
            void set_tag(void_ptr tag) { tag_ = tag; }
            void_ptr tag() { return tag_; }
            
            void set_remoteaddr(const std::string& addr) { remoteaddr_ = addr; }
            std::string& remoteaddr() { return remoteaddr_; }
            
            void set_localaddr(const std::string& addr) { localaddr_ = addr; }
            std::string& localaddr() { return localaddr_; }
            
            tcpsession_type sessiontype() { return stype_; }
            
            void set_active() { time(&last_active_time_); }
            bool timeout();
            
            int pack_parse_complete(decoder* pack);
            int line_parse_complete(std::string& cmd, std::vector<std::string>& args);
            void send(encoder* pack);
            void receive();
            void close_without_notify();
            
            void send_prompt();
            void send_line(const char* data, int len);
            
            int set_sndbuf_size(int size, bool force=false);
            int set_rcvbuf_size(int size, bool force=false);
            
        private:
	        iparser* create_parser(parser_type type);
            void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
            void handle_write(const boost::system::error_code& error);
            void handle_close();
            
        private:
            boost::asio::ip::tcp::socket socket_;
	        boost::scoped_ptr<iparser> parser_;
            message_callback messagecb_;
            close_callback closecb_;
            int sessionid_;
            void_ptr tag_;
            std::string remoteaddr_;
            std::string localaddr_;
            tcpsession_type stype_;
            parser_type parsetype_;
            std::time_t last_active_time_;
            char data_[c_buffer_size];
        };
        
    }
}

#endif

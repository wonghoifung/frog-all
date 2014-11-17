//
//  tcpsession.cpp
//  console
//
//  Created by huanghaifeng on 14-10-2.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "tcpsession.h"
#include "tcpsessionmgr.h"
#include "linecmd_mgr.h"

namespace frog
{
    namespace generic
    {
        
        class sessionid_generator : public boost::noncopyable
        {
        private:
            sessionid_generator():id_(0){}
            ~sessionid_generator(){}
        public:
            static sessionid_generator& ref()
            {
                static sessionid_generator sg;
                return sg;
            }
            int gen()
            {
                do {
                    ++id_;
                    if (id_ < 0) { id_ = 1; }
                } while (tcpsessionmgr::ref().find(id_));
                return id_;
            }
        private:
            int id_;
        };

#if _WIN32
	tcpsession::tcpsession(boost::asio::io_service& io_service, tcpsession_type stype, parser_type ptype)
            : socket_(io_service),
#else
        tcpsession::tcpsession(const boost::asio::io_service& io_service, const tcpsession_type stype, const parser_type ptype)
            : socket_(const_cast<boost::asio::io_service&>(io_service)),
#endif
              sessionid_(sessionid_generator::ref().gen()),
              stype_(stype), parsetype_(ptype), last_active_time_(INT_MAX)
        {
            //std::cout << "tcpsession ctor, id:" << sessionid_ << std::endl;
            
            /* pass raw tcpsession pointer to prevent circular reference */
	        parser_.reset(create_parser(parsetype_));
        }
        
        tcpsession::~tcpsession()
        {
            //std::cout << "tcpsession dtor, id:" << sessionid_ << std::endl;
        }
        
        boost::asio::ip::tcp::socket& tcpsession::socket()
        {
            return socket_;
        }
        
        void tcpsession::start()
        {
            boost::system::error_code e;
            boost::asio::socket_base::keep_alive op1(true);
            socket().set_option(op1, e);
            boost::asio::ip::tcp::no_delay op2(true);
            socket().set_option(op2, e);
            time(&last_active_time_);
            if (parsetype_ == parse_line)
            { send_prompt(); }
            receive();
        }
        
        bool tcpsession::timeout()
        {
            const static int conntimeout = 60;
            return (time(NULL) - last_active_time_ > conntimeout);
        }
        
        int tcpsession::pack_parse_complete(decoder* pack)
        {
            if (messagecb_)
            {
                if (pack->command() == cmd_heart_beat && stype_ == tcpsession_server)
                { set_active(); }
                else
                { messagecb_(shared_from_this(), pack); /* no need to set active */ }
            }
            return 0;
        }
        
        int tcpsession::line_parse_complete(std::string& cmd, std::vector<std::string>& args)
        {
            set_active(); 
            linecmd_mgr::ref().dispatch(cmd, args, shared_from_this());
            send_prompt();
            return 0;
        }
        
        void tcpsession::send(encoder* pack)
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(pack->cbuffer(), pack->size()),
                                     boost::bind(&tcpsession::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error));
        }
        
        void tcpsession::receive()
        {
            socket_.async_read_some(boost::asio::buffer(data_, c_buffer_size),
                                    boost::bind(&tcpsession::handle_read, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
        
        void tcpsession::close_without_notify()
        {
            socket().close();
        }
        
        void tcpsession::send_prompt()
        {
            std::string prompt = "> ";
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(prompt.c_str(), prompt.size()),
                                     boost::bind(&tcpsession::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error));
        }
        
        void tcpsession::send_line(const char* data, int len)
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(data, len),
                                     boost::bind(&tcpsession::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error));
        }
        
        int tcpsession::set_sndbuf_size(int size, bool force)
        {
            boost::system::error_code e;
            if (force)
            {
                boost::asio::socket_base::send_buffer_size op(size);
                socket().set_option(op, e);
            }
            else
            {
                boost::asio::socket_base::send_buffer_size sbs;
                socket().get_option(sbs, e);
                
                if (e.value() != 0 || sbs.value() < size)
                {
                    boost::asio::socket_base::send_buffer_size op(size);
                    socket().set_option(op, e);
                }
            }
            return e.value();
        }
        
        int tcpsession::set_rcvbuf_size(int size, bool force)
        {
            boost::system::error_code e;
            if (force == true)
            {
                boost::asio::socket_base::receive_buffer_size  op(size);
                socket().set_option(op, e);
            }
            else
            {
                boost::asio::socket_base::receive_buffer_size rbs;
                socket().get_option(rbs, e);
                
                if (e.value() != 0 || rbs.value() < size)
                {
                    boost::asio::socket_base::receive_buffer_size  op(size);
                    socket().set_option(op, e);
                }
            }
            return e.value();
        }
        
		iparser* tcpsession::create_parser(parser_type type)
		{
			switch (type)
			{
			case frog::generic::parse_pack:
				return new packparser(boost::bind(&tcpsession::pack_parse_complete, this, _1));
			case frog::generic::parse_line:
				return new lineparser(boost::bind(&tcpsession::line_parse_complete, this, _1, _2));
			default:
				break;
			}
			return NULL;
		}

        void tcpsession::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
        {
            if (!error)
            {
		        if (parser_.get())
		        { parser_->parse(data_, bytes_transferred); }
                
                receive();
            }
            else
            {
                handle_close();
            }
        }
        
        void tcpsession::handle_write(const boost::system::error_code& error)
        {
            if (error)
            { handle_close(); }
        }
        
        void tcpsession::handle_close()
        {
            socket().close();
            if (closecb_)
            { closecb_(shared_from_this()); }
            tcpsessionmgr::ref().erase(sessionid());
        }
        
    }
}

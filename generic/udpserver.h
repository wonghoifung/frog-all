//
//  udpserver.h
//  console
//
//  Created by huanghaifeng on 14-10-4.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef udpserver_header
#define udpserver_header

#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace frog
{
    namespace generic
    {
        
        class udpserver;
        typedef boost::function<void(char*,int,udpserver&)> dgram_callback;
        
        class udpserver : public boost::noncopyable
        {
        public:
            udpserver(boost::asio::io_service& io_service, short port);
            void run();
            void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);
            void handle_send_to(const boost::system::error_code& /*error*/, size_t /*bytes_sent*/);
            void set_dgramcb(dgram_callback cb) { dgramcb_ = cb; }
            void sendback(const char* data, int len);
            std::string remoteaddr();
            std::string localaddr();
            
        private:
            boost::asio::ip::udp::socket socket_;
            boost::asio::ip::udp::endpoint sender_endpoint_;
            dgram_callback dgramcb_;
            enum { max_length = 1024 };
            char data_[max_length];
        };
        
    }
}

#endif

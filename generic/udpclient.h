//
//  udpclient.h
//  console
//
//  Created by huanghaifeng on 14-10-4.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef udpclient_header
#define udpclient_header

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace frog
{
    namespace generic
    {
     
        class udpclient;
        typedef boost::function<void(char*,int,udpclient&)> client_dgram_callback;
        
        class udpclient : public boost::noncopyable
        {
        public:
            udpclient(boost::asio::io_service& io_service, const char* ip, const char* port);
            void handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd);
            void handle_send_to(const boost::system::error_code& /*error*/, size_t /*bytes_sent*/);
            void set_client_dgramcb(client_dgram_callback cb) { dgramcb_ = cb; }
            void send(const char* data, size_t len);
            void send_without_receiveback(const char* data, size_t len);
            std::string remoteaddr();
            std::string localaddr();
            
        private:
            boost::asio::ip::udp::socket socket_;
            boost::asio::ip::udp::endpoint server_endpoint_;
            client_dgram_callback dgramcb_;
            enum { max_length = 1024 };
            char data_[max_length];
        };
        
    }
}

#endif

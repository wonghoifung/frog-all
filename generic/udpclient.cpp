//
//  udpclient.cpp
//  console
//
//  Created by huanghaifeng on 14-10-4.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <boost/format.hpp>
#include "udpclient.h"

using boost::asio::ip::udp;

namespace frog
{
    namespace generic
    {
        
        udpclient::udpclient(boost::asio::io_service& io_service, const char* ip, const char* port)
            : socket_(io_service)
        {
            udp::resolver resolver(io_service);
            udp::resolver::query query(udp::v4(), ip, port);
            server_endpoint_ = *resolver.resolve(query);
            socket_.open(udp::v4());
        }
        
        void udpclient::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
        {
            if (!error && bytes_recvd > 0)
            {
                if (dgramcb_)
                { dgramcb_(data_, (int)bytes_recvd, *this); }
            }
            else
            {
                socket_.async_receive_from(boost::asio::buffer(data_, max_length), server_endpoint_,
                                           boost::bind(&udpclient::handle_receive_from, this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));
            }
        }
        
        void udpclient::handle_send_to(const boost::system::error_code& /*error*/, size_t /*bytes_sent*/)
        {
            socket_.async_receive_from(boost::asio::buffer(data_, max_length), server_endpoint_,
                                       boost::bind(&udpclient::handle_receive_from, this,
                                                   boost::asio::placeholders::error,
                                                   boost::asio::placeholders::bytes_transferred));
        }
        
        void udpclient::send(const char* data, size_t len)
        {
            socket_.async_send_to(boost::asio::buffer(data, len), server_endpoint_,
                                  boost::bind(&udpclient::handle_send_to, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
            
            /*
            socket_.send_to(boost::asio::buffer(data, len), server_endpoint_);
            char reply[max_length];
            udp::endpoint sender_endpoint;
            size_t reply_length = socket_.receive_from(boost::asio::buffer(reply, max_length),
                                                       sender_endpoint);
            if (reply_length > 0 && dgramcb_)
            { dgramcb_(reply, (int)reply_length, *this); }
             */
        }
        
        void _____send_done(const boost::system::error_code& /*error*/, size_t /*bytes_sent*/){}
        void udpclient::send_without_receiveback(const char* data, size_t len)
        {
            socket_.async_send_to(boost::asio::buffer(data, len), server_endpoint_,
                                  boost::bind(&_____send_done,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
            
            //socket_.send_to(boost::asio::buffer(data, len), server_endpoint_);
        }
        
        std::string udpclient::remoteaddr()
        {
            boost::format fmt("%1%:%2%");
            fmt % server_endpoint_.address().to_string();
            fmt % server_endpoint_.port();
            return fmt.str();
        }
        
        std::string udpclient::localaddr()
        {
            boost::format fmt("%1%:%2%");
            fmt % socket_.local_endpoint().address().to_string();
            fmt % socket_.local_endpoint().port();
            return fmt.str();
        }
        
    }
}

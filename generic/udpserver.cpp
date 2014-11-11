//
//  udpserver.cpp
//  console
//
//  Created by huanghaifeng on 14-10-4.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <boost/format.hpp>
#include "udpserver.h"

using boost::asio::ip::udp;

namespace frog
{
    namespace generic
    {
        
        udpserver::udpserver(boost::asio::io_service& io_service, short port)
            : socket_(io_service, udp::endpoint(udp::v4(), port))
        {
        }
        
        void udpserver::run()
        {
            socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
                                       boost::bind(&udpserver::handle_receive_from, this,
                                                   boost::asio::placeholders::error,
                                                   boost::asio::placeholders::bytes_transferred));
        }
        
        void udpserver::handle_receive_from(const boost::system::error_code& error, size_t bytes_recvd)
        {
            if (!error && bytes_recvd > 0)
            {
                if (dgramcb_)
                { dgramcb_(data_, (int)bytes_recvd, *this); }
            }
            else
            {
                socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
                                           boost::bind(&udpserver::handle_receive_from, this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));
            }
        }
        
        void udpserver::handle_send_to(const boost::system::error_code& /*error*/, size_t /*bytes_sent*/)
        {
            socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
                                       boost::bind(&udpserver::handle_receive_from, this,
                                                   boost::asio::placeholders::error,
                                                   boost::asio::placeholders::bytes_transferred));
        }
        
        void udpserver::sendback(const char* data, int len)
        {
            socket_.async_send_to(boost::asio::buffer(data_, len), sender_endpoint_,
                                  boost::bind(&udpserver::handle_send_to, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
        }
        
        std::string udpserver::remoteaddr()
        {
            boost::format fmt("%1%:%2%");
            fmt % sender_endpoint_.address().to_string();
            fmt % sender_endpoint_.port();
            return fmt.str();
        }
        
        std::string udpserver::localaddr()
        {
            boost::format fmt("%1%:%2%");
            fmt % socket_.local_endpoint().address().to_string();
            fmt % socket_.local_endpoint().port();
            return fmt.str();
        }
        
    }
}


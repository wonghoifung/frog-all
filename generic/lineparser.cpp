//
//  lineparser.cpp
//  console
//
//  Created by huanghaifeng on 14-10-3.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include <sstream>
#include "lineparser.h"

namespace frog
{
    namespace generic
    {
        
        lineparser::lineparser(command_callback cb) : commandcb_(cb)
        {
            init();
        }
        
        lineparser::~lineparser()
        {
        }
        
        int lineparser::parse(const char* data, const size_t length)
        {
            buffer_.append(std::string(data, length));
            std::size_t found = buffer_.find('\n');
            if (found != std::string::npos)
            {
                std::string line = buffer_.substr(0, found);
                
                /* leftover, exclude '\n' */
                buffer_ = buffer_.substr(found + 1); 
                
                /* remove all useless char */
                while ('\n' == *line.rbegin() || '\r' == *line.rbegin())
                { line = line.substr(0, line.size() - 1); }
                
                std::stringstream ss(line);
                ss >> command_;
                std::string arg;
                while (ss >> arg) { arguments_.push_back(arg); }
                if (commandcb_)
                {
                    commandcb_(command_, arguments_);
                    init();
                }
            }
            return 0;
        }
        
        void lineparser::init()
        {
            command_ = "";
            arguments_.clear();
        }
        
    }
}


//
//  lineparser.h
//  console
//
//  Created by huanghaifeng on 14-10-3.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef lineparser_header
#define lineparser_header

#include <boost/function.hpp>
#include <string>
#include <vector>
#include "iparser.h"

namespace frog
{
    namespace generic
    {
        
        class lineparser : public iparser
        {
            typedef boost::function<int(std::string&,std::vector<std::string>&)> command_callback;
            
        public:
            lineparser(command_callback cb);
            ~lineparser();
            virtual int parse(const char* data, const size_t length);
            
        private:
            void init();
            
        private:
            command_callback commandcb_;
            std::string buffer_;
            std::string command_;
            std::vector<std::string> arguments_;
        };
        
    }
}

#endif 

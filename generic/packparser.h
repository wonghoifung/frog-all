//
//  packparser.h
//  console
//
//  Created by huanghaifeng on 14-10-2.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef packparser_header
#define packparser_header

#include <boost/function.hpp>
#include "decoder.h"
#include "iparser.h"

namespace frog
{
    namespace generic
    {
        
        class packparser : public iparser
        {
            typedef boost::function<int(decoder*)> complete_callback;
            enum { c_header=0, c_body, c_done, c_error };
            
        public:
            packparser(complete_callback cb);
            ~packparser();
            virtual int parse(const char* data, const size_t length);
            void init();
            
        private:
            bool read_header(const char* data, const size_t length, size_t& ndx);
            int parse_header();
            bool parse_body(const char* data, const size_t length, size_t& ndx);
            
        private:
            complete_callback complete_cb_;
            int state_;
            size_t	packpos_;
            size_t bodylen_;
            char* buf_;
            decoder decoder_;
        };
        
    }
}

#endif

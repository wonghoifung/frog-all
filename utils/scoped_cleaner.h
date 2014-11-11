//
//  scoped_cleaner.h
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef scoped_cleaner_header
#define scoped_cleaner_header

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <stdio.h>

namespace frog
{
    namespace utils
    {
        
        class scoped_cleaner : public boost::noncopyable
        {
            typedef boost::function<void()> clean_callback;
            
        public:
            explicit scoped_cleaner(clean_callback cb) : cleancb_(cb)
            {
                
            }
            
            ~scoped_cleaner()
            {
                //printf("call scoped cleaner\n");
                if (cleancb_) { cleancb_(); }
            }
            
        private:
            clean_callback cleancb_;
        };
        
    }
}

#endif

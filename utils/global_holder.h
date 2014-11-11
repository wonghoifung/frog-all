//
//  global_holder.h
//  console
//
//  Created by huanghaifeng on 14-10-3.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef global_holder_header
#define global_holder_header

#include <boost/noncopyable.hpp>
#include <boost/assert.hpp>

namespace frog
{
    namespace utils
    {
        
        template <typename T>
        class global_holder : boost::noncopyable
        {
        private:
            global_holder():holdee_(NULL){}
            ~global_holder(){}
            
        public:
            static global_holder<T>& ref()
            {
                static global_holder<T> instance;
                return instance;
            }
            
            void set(T* t) { holdee_ = t; }
            T& get() { BOOST_ASSERT(holdee_); return *holdee_; }
            
        private:
            T* holdee_;
        };
        
    }
}

#endif

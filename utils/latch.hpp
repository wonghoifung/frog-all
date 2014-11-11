//
//  latch.hpp
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef latch_header
#define latch_header

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace frog
{
    namespace utils
    {
        
        class latch : public boost::noncopyable
        {
        public:
            explicit latch(int count) : count_(count)
            {
                
            }
            
            void wait()
            {
                boost::mutex::scoped_lock lock(mutex_);
                while (count_ > 0) { conditionvar_.wait(lock); }
            }
            
            void done()
            {
                boost::mutex::scoped_lock lock(mutex_);
                --count_;
                if (count_ == 0) { conditionvar_.notify_all(); }
            }
            
            int left() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return count_;
            }
            
        private:
            mutable boost::mutex mutex_;
            boost::condition_variable conditionvar_;
            int count_;
        };
        
    }
}

#endif 



//
//  mrmw_queue.hpp
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

/* multiple reads multiple writes blocking queue */

#ifndef mrmw_queue_header
#define mrmw_queue_header

#include <queue>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace frog
{
    namespace utils
    {
        
        template<typename T>
        class mrmw_queue : boost::noncopyable
        {
        public:
            void push(const T& t)
            {
                boost::mutex::scoped_lock lock(mutex_);
                queue_.push(t);
                lock.unlock();
                conditionvar_.notify_one();
            }
            
            T pop()
            {
                boost::mutex::scoped_lock lock(mutex_);
                while(queue_.empty()) { conditionvar_.wait(lock); }
                T ret(queue_.front());
                queue_.pop();
                return ret;
            }
            
            size_t size() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return queue_.size();
            }
            
        private:
            mutable boost::mutex mutex_;
            boost::condition_variable conditionvar_;
            std::queue<T> queue_;
        };
        
    }
}

#endif

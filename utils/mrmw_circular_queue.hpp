//
//  mrmw_circular_queue.hpp
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

/* multiple reads multiple writes blocking circular queue */

#ifndef mrmw_circular_queue_header
#define mrmw_circular_queue_header

#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace frog
{
    namespace utils
    {
        
        template<typename T>
        class mrmw_circular_queue : boost::noncopyable
        {
        public:
            explicit mrmw_circular_queue(int size) : queue_(size)
            {
            }
            
            void push(const T& t)
            {
                boost::mutex::scoped_lock lock(mutex_);
                while (queue_.full()) { datago_.wait(lock); }
                queue_.push_back(t);
                lock.unlock();
                datacome_.notify_one();
            }
            
            T pop()
            {
                boost::mutex::scoped_lock lock(mutex_);
                while (queue_.empty()) { datacome_.wait(lock); }
                T ret(queue_.front());
                queue_.pop_front();
                lock.unlock();
                datago_.notify_one();
                return ret;
            }
            
            bool empty() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return queue_.empty();
            }
            
            bool full() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return queue_.full();
            }
            
            size_t size() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return queue_.size();
            }
            
            size_t capacity() const
            {
                boost::mutex::scoped_lock lock(mutex_);
                return queue_.capacity();
            }
            
        private:
            mutable boost::mutex mutex_;
            boost::condition_variable datacome_;
            boost::condition_variable datago_;
            boost::circular_buffer<T> queue_;
        };
        
    }
}

#endif

//
//  timer.h
//  console
//
//  Created by huanghaifeng on 14-10-30.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef timer_header
#define timer_header

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <set>
#include <assert.h>

namespace frog
{
    namespace utils
    {
        
        class timer;
        typedef boost::function<void(int,timer*)> timeout_callback;
        
        class timer : public boost::noncopyable
        {
            struct task
            {
                int task_id;
                time_t expiry_time;
                timeout_callback callback;
            };
            
            struct task_comp
            {
                bool operator()(const task& left, const task& right)
                {
                    if( left.expiry_time != right.expiry_time )
                        return left.expiry_time < right.expiry_time;
                        
                    return left.task_id < right.task_id;
                }  
            };
                        
            typedef std::set<task, task_comp> task_set;
            
        public:
            timer(boost::asio::io_service& ioserv);
            ~timer();
            void add_task(int tid, time_t dura, timeout_callback cb);
            void del_task(int tid);
            
        private:
            void on_timeout(const boost::system::error_code & error);
            void start_timer(int dura);
            void stop_timer();
            int calc_first_dura(time_t now);
            bool is_task_existed(int tid);
            void set_task_existed(int tid);
            void clear_task_existed(int tid);
                        
        private:
            boost::asio::deadline_timer general_timer_;
            task_set tasks_;
            unsigned int flag_;
            bool start_;
        };
        
    }
}

#endif


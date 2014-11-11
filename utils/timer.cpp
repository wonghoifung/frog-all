//
//  timer.cpp
//  console
//
//  Created by huanghaifeng on 14-10-30.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "timer.h"
#include <boost/bind.hpp>

namespace frog
{
    namespace utils
    {
        
        timer::timer(boost::asio::io_service& ioserv)
            :general_timer_(ioserv)
            ,flag_(0)
            ,start_(false)
        {
            
        }
        
        timer::~timer()
        {
            stop_timer();
        }
        
        void timer::add_task(int tid, time_t dura, timeout_callback cb)
        {
            if (is_task_existed(tid)) {
                //printf("cannot add same task:%d\n",tid);
                return;
            }
            
            if(start_)stop_timer();
            time_t now(time(NULL));
            time_t deadline = now + dura;
            task t = {tid, deadline, cb};
            std::pair<task_set::iterator,bool> ret = tasks_.insert(t);
            assert(ret.second);
            set_task_existed(tid);
            start_timer(calc_first_dura(now));
        }
        
        void timer::del_task(int tid)
        {
            task_set::iterator beg(tasks_.begin()), end(tasks_.end());
            for (; beg != end; ++beg)
            {
                if (beg->task_id == tid)
                {
                    stop_timer();
                    tasks_.erase(beg);
                    clear_task_existed(tid);
                    start_timer(calc_first_dura(time(NULL)));
                    return;
                }
            }
        }
        
        void timer::on_timeout(const boost::system::error_code & error)
        {
            start_ = false;
            if (error)
            {
                //printf("cancel timer\n");
                return;
            }
            
            time_t now(time(NULL));
            while (tasks_.size())
            {
                task_set::iterator beg(tasks_.begin());
                if (beg->expiry_time > now) { break; }
                const task t = *beg;
                tasks_.erase(beg);
                clear_task_existed(t.task_id);
                if (t.callback) { t.callback(t.task_id,this); }
            }
            
            if (tasks_.size() && !start_)
            {
                start_timer(calc_first_dura(now));
            }
        }
        
        void timer::start_timer(int dura)
        {
            start_ = true;
            boost::system::error_code e;
            general_timer_.expires_from_now(boost::posix_time::seconds(dura), e);
            general_timer_.async_wait(boost::bind(&timer::on_timeout,
                                                  this,
                                                  boost::asio::placeholders::error));
        }
        
        void timer::stop_timer()
        {
            start_ = false;
            boost::system::error_code e;
            general_timer_.cancel(e);
        }
        
        int timer::calc_first_dura(time_t now)
        {
            task_set::iterator beg(tasks_.begin());
            time_t first_deadline = beg->expiry_time;
            time_t first_dura = first_deadline - now;
            if (first_dura <= 0) { first_dura = 1; }// TODO refine
            return (int)first_dura;
        }
        
        bool timer::is_task_existed(int tid)
        {
            if (flag_ & (1u << tid))
            {
                return true;
            }
            return false;
        }
        
        void timer::set_task_existed(int tid)
        {
            flag_ |= (1u << tid);
        }
        
        void timer::clear_task_existed(int tid)
        {
            flag_ &= (~(1u << tid));
        }
        
    }
}
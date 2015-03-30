//
//  luathread.h
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014 wonghoifung. All rights reserved.
//

#ifndef luathread_header
#define luathread_header

#include <string>
#include <cstdarg>
#ifdef __APPLE__
#include <thread>
#else
#include <boost/thread.hpp>
#endif
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/atomic.hpp>
#include "latch.hpp"
#include "mrmw_queue.hpp"
#include "PacketBaseAdapter.h"
#include "luahelper.h"

namespace frog
{
    namespace utils
    {
       
        class luathread : public boost::noncopyable
        {
#ifdef __APPLE__
            typedef std::thread thread_t;
#else
            typedef boost::thread thread_t;
#endif
            typedef boost::shared_ptr<thread_t> thread_ptr;
            
        public:
            luathread();
            void init();
            void join() { thread_->join(); }
            void pushmsg(NETInputPacket* pack);
            
            //script should stop first before setstop(true) is called
            void setstop(bool s) {stop_=s;} 
           
            void setmsgcnt(int cnt) {msgcnt_=cnt;}
            int getmsgcnt() {return msgcnt_;}
 
        private:
            void run();
            
        private:
            boost::atomic_bool stop_;
            boost::atomic_int msgcnt_;
            thread_ptr thread_;
            latch initdone_;
            luahelper luahelp_;
            mrmw_queue<NETInputPacket*> packs_;
        };
        
    }
}

#endif



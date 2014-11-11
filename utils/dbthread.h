//
//  dbthread.h
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef dbthread_header
#define dbthread_header

#include <string>
#include <cstdarg>
#ifdef __APPLE__
#include <thread>
#else
#include <boost/thread.hpp>
#endif
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "idbwrapper.h"
#include "dboperator.h"
#include "latch.hpp"
#include "mrmw_queue.hpp"

namespace frog
{
    namespace utils
    {
       
        class dbthread : public boost::noncopyable
        {
#ifdef __APPLE__
            typedef std::thread thread_t;
#else
            typedef boost::thread thread_t;
#endif
            typedef boost::shared_ptr<thread_t> thread_ptr;
            typedef boost::function<void(int, db::dataset*)> dbquery_callback;
            typedef boost::function<void(int, bool)> dbexecute_callback;
            typedef boost::function<void(int, std::string)> dberror_callback;
            typedef struct 
            {
                int tag;
                bool isquery;
                std::string sql;
            } sql_t;
            
        public:
            dbthread(db::dbtype dbt, const db::conn_args_t& args);
            void init();
            void join() { thread_->join(); }
            void set_error_callback(dberror_callback cb) { errorcb_ = cb; }
            void set_query_callback(dbquery_callback cb) { querycb_ = cb; }
            void set_execute_callback(dbexecute_callback cb) { executecb_ = cb; }
            void execute(int tag, const char* sql, ...);
            void query(int tag, const char* sql, ...);
            
        private:
            void run();
            
        private:
            thread_ptr thread_;
            latch initdone_;
            db::dboperator operator_;
            db::conn_args_t connargs_;
            mrmw_queue<sql_t> sqls_;
            
            dberror_callback errorcb_;
            dbquery_callback querycb_;
            dbexecute_callback executecb_;
        };
        
    }
}

#endif

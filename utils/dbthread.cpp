//
//  dbthread.cpp
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "dbthread.h"
#include <boost/make_shared.hpp>

namespace frog
{
    namespace utils
    {
       
        dbthread::dbthread(db::dbtype dbt, const db::conn_args_t& args)
            : initdone_(1), operator_(dbt), connargs_(args)
        {
        }
        
        void dbthread::init()
        {
            thread_ = boost::make_shared<thread_t>(boost::bind(&dbthread::run, this));
            initdone_.wait();
        }
        
        void dbthread::execute(int tag, const char* sql, ...)
        {
            char buff[SQLBUF_MAXLEN] = {0};
            va_list ap;
            va_start(ap, sql);
            int ret = vsnprintf(buff, SQLBUF_MAXLEN, sql, ap);
            va_end(ap);
            if( -1 == ret)
            {
                if (errorcb_) { errorcb_(tag, "cannot build sql"); }
                return;
            }
            
            sql_t sqlt;
            sqlt.tag = tag;
            sqlt.sql = buff;
            sqlt.isquery = false;
            sqls_.push(sqlt);
        }
        
        void dbthread::query(int tag, const char* sql, ...)
        {
            char buff[SQLBUF_MAXLEN] = {0};
            va_list ap;
            va_start(ap, sql);
            int ret = vsnprintf(buff, SQLBUF_MAXLEN, sql, ap);
            va_end(ap);
            if( -1 == ret)
            {
                if (errorcb_) { errorcb_(tag, "cannot build sql"); }
                return;
            }
            
            sql_t sqlt;
            sqlt.tag = tag;
            sqlt.sql = buff;
            sqlt.isquery = true;
            sqls_.push(sqlt);
        }
        
        void dbthread::run()
        {
            bool success = operator_.connect(connargs_);
            initdone_.done();
            if (!success)
            {
                if (errorcb_)
                {
                    errorcb_(-1, "cannot connect in dbthread");
                }
                return;
            }
            
            /* run forever */
            while (true)
            {
                sql_t sql = sqls_.pop();
                if (sql.isquery)
                {
                    db::dataset* ds = operator_.query(sql.sql.c_str());
                    if (querycb_)
                    {
                        querycb_(sql.tag, ds);
                    }
                }
                else
                {
                    bool ret = operator_.execute(sql.sql.c_str());
                    if (executecb_)
                    {
                        executecb_(sql.tag, ret);
                    }
                }
            }
        }
        
    }
}


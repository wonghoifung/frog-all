//
//  logger.h
//  console
//
//  Created by huanghaifeng on 14-11-1.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef logger_header
#define logger_header

#include <stdio.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace frog
{
    namespace utils
    {
        
        class logger : public boost::noncopyable
        {
        private:
            logger();
            ~logger();
            
        public:
            static logger& ref();
            bool init(const std::string& fname, const std::string& path);
            bool write(const std::string& prefix1,
                       const std::string& prefix2,
                       int prefix3,
                       const char* fmt, ...);
            bool write_hex(const std::string& prefix1,
                           const std::string& prefix2,
                           int prefix3,
                           const char* buf, int len);
            
        private:
            FILE* getfile();
            FILE* openfile();
            
        private:
            std::string basename_;
        };
        
    }
}
#define logstr(fmt,args...) frog::utils::logger::ref().write(__FILE__,__FUNCTION__,__LINE__,fmt,##args)
#define loghex(buf,len) frog::utils::logger::ref().write_hex(__FILE__,__FUNCTION__,__LINE__,buf,len)

#endif 


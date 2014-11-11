//
//  logger.cpp
//  console
//
//  Created by huanghaifeng on 14-11-1.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "logger.h"
#include "time_utils.h"
#include <stdarg.h>
#include <sys/stat.h>

#ifdef _WIN32
#define vsnprintf _vsnprintf
#endif

namespace
{
    int print(char* buf, int maxlen, const char* fmt, ...)
    {
        if (!buf) { return 0; }
        
        int cnt = 0;
        va_list arglist;
        
        va_start(arglist,fmt);
        cnt += vsnprintf(buf+cnt,maxlen-cnt,fmt,arglist);
        va_end(arglist);
        
        if (cnt > (maxlen-1))
        { cnt = maxlen - 1; }
        
        *(buf+cnt) = '\0';
        return cnt;
    }
}

namespace frog
{
    namespace utils
    {
        
        logger::logger()
        {
            
        }
        
        logger::~logger()
        {
            
        }
        
        logger& logger::ref()
        {
            static logger l;
            return l;
        }
        
        bool logger::init(const std::string& fname, const std::string& path)
        {
            mkdir(path.c_str(),0777);
            if (access(path.c_str(), W_OK|X_OK) < 0)
            {
                fprintf(stderr, "log path:%s not writable\n", path.c_str());
                return false;
            }
            if (path[path.size()-1] != '/')
            { basename_ = path + "/" + fname; }
            else
            { basename_ = path + fname; }
            return true;
        }
        
        bool logger::write(const std::string& prefix1,
                           const std::string& prefix2,
                           int prefix3,
                           const char* fmt, ...)
        {
            FILE* f = getfile();
            if (!f) { return false; }
            std::string sourcefile = prefix1.substr(prefix1.find_last_of("/")+1);
            
            const int buflen = 2048;
            char buf[buflen];
            int cnt(0);
            
            va_list arglist;
            va_start(arglist, fmt);
            cnt += vsnprintf(buf+cnt, buflen-cnt, fmt, arglist);
            va_end(arglist);
            
            if (buf[cnt - 1] != '\n')
            {
                if (cnt > (buflen - 2))
                { cnt = buflen - 2; }
                *(buf + cnt) = '\n';
                *(buf + cnt + 1) = '\0';
            }
            else
            {
                if (cnt > (buflen - 1))
                { cnt = buflen - 1; }
                *(buf + cnt) = '\0';
            }
            
            fprintf(f, "|%s|%s|%s|%d|: %s",
                    get_string_time(time(NULL)).c_str(),
                    sourcefile.c_str(),
                    prefix2.c_str(),
                    prefix3,
                    buf);
            fflush(f);
            return true;
        }
        
        bool logger::write_hex(const std::string& prefix1,
                               const std::string& prefix2,
                               int prefix3,
                               const char* buf, int len)
        {
            FILE* f = getfile();
            if (!f) { return false; }
            
            std::string sourcefile = prefix1.substr(prefix1.find_last_of("/")+1);
            std::string now = get_string_time(time(NULL));
            
            int offset = 0;
            int bufleft = len;
            int n = 0;
            const int linesize = 256;
            char line[linesize];
            if (len > 0)
            {
                fprintf(f, "|%s|%s|%s|%d|: hexadecimal format...\n",
                        now.c_str(),
                        sourcefile.c_str(),
                        prefix2.c_str(),
                        prefix3);
                
                while (true)
                {
                    n = 0;
                    n += print(line+n, linesize-n, "%p - ", buf+offset);
                    
                    const char* inbuf = buf + offset;
                   
                    /* print hex */
                    if (bufleft < 16)
                    {
                        int i(0);
                        int j(0);
                        for (; i<bufleft; ++i)
                        {
                            n += print(line+n, linesize-n, "%02x ", inbuf[i]);
                            ++j;
                            if (4 == j)
                            {
                                j = 0;
                                n += print(line+n, linesize-n, " ");
                            }
                            
                        }
                        for (; i < 16; ++i)
                        {
                            n += print(line+n, linesize-n, "   ");
                            ++j;
                            if (4 == j)
                            {
                                j = 0;
                                n += print(line+n, linesize-n, " ");
                            }
                        }
                    }
                    else
                    {
                        n += print(line+n,linesize,
                                   "%02x %02x %02x %02x  "
                                   "%02x %02x %02x %02x  "
                                   "%02x %02x %02x %02x  "
                                   "%02x %02x %02x %02x  ",
                                   inbuf[0],inbuf[1],inbuf[2],inbuf[3],
                                   inbuf[4],inbuf[5],inbuf[6],inbuf[7],
                                   inbuf[8],inbuf[9],inbuf[10],inbuf[11],
                                   inbuf[12],inbuf[13],inbuf[14],inbuf[15]);
                    }
                    
                    int charcnt = (bufleft < 16 ? bufleft : 16);
                    /* print char */
                    for (int i=0; i<charcnt; ++i)
                    {
                        if (*(inbuf + i) >= 32)
                        {
                            /* printable */
                            n += print(line+n, linesize-n, "%c", *(inbuf+i));
                        }
                        else
                        {
                            /* replace with '.' while not printable */
                            n += print(line+n, linesize-n, ".");
                        }
                    }
                   
                    /* write file */
                    fprintf(f, "    %s\n", line);
                    
                    offset += 16;
                    bufleft -= 16;
                    if (bufleft <= 0) { break; }
                }
                fflush(f);
                return true;
            }
            return false;
        }
        
        FILE* logger::getfile()
        {
            static FILE* f = NULL;
            
            if (!f)
            {
                f = openfile();
                return f;
            }
            
            fseek(f, 0L, SEEK_END);
            if (ftell(f) >= (50 * 1024 * 1024)) /* no bigger than 50M */
            {
                fclose(f);
                f = openfile();
                return f;
            }
            
            return f;
        }
        
        FILE* logger::openfile()
        {
            time_t now = time(NULL);
            int year(0),month(0),day(0),hour(0),minute(0),second(0);
            get_time_details(&now, year, month, day, hour, minute, second);
            char buf[256] = {0};
            snprintf(buf, 256, "%s%d%02d%02d%02d%02d%02d.log",
                     basename_.c_str(),year,month,day,hour,minute,second);
            std::string logfile(buf);
            FILE* f = fopen(logfile.c_str(), "a+");
            if (!f)
            {
                fprintf(stderr, "cannot open logfile:%s, error:%s",
                        logfile.c_str(),strerror(errno));
                return NULL;
            }
            return f;
        }
        
    }
}
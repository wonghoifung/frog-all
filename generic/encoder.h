//
//  encoder.h
//  console
//
//  Created by huanghaifeng on 14-10-1.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef encoder_header
#define encoder_header

#include "codec.h"

namespace frog
{
    namespace generic
    {
        
        class encoder : public codec
        {
        public:
            bool write_int(int val);
            bool write_uint(unsigned int val);
			bool write_int64(int64_t val);
            bool write_uint64(uint64_t val);
			bool write_long(long val);
            bool write_ulong(unsigned long val);
			bool write_char(char val);
            bool write_uchar(unsigned char val);
            bool write_short(short val);
			bool write_ushort(unsigned short val);
            bool write_front_int(int val);
			bool write_front_uint(unsigned int val);
			bool write_front_char(char val);
            bool write_front_uchar(unsigned char val);
            bool write_cstring(const char* cstr);
            bool write_string(const std::string& str);
            bool write_binary(const char* inbuf, int len);
            
            bool copy(const void* inbuf, int len);
            void begin(short cmd, char ver = c_default_version, char subver = c_default_subversion);
            void end();
        };
        
    }
}

#endif


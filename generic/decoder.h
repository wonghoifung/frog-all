//
//  decoder.h
//  console
//
//  Created by huanghaifeng on 14-10-1.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef decoder_header
#define decoder_header

#include "codec.h"

namespace frog
{
    namespace generic
    {
        
        class decoder : public codec
        {
        public:
            int            read_int();
            int            read_int_and_repack();
            unsigned int   read_uint();
			int64_t        read_int64();
			uint64_t       read_uint64();
			long           read_long();
            unsigned long  read_ulong();
            short          read_short();
			unsigned short read_ushort();
			char           read_char();
            unsigned char  read_uchar();
            char*          read_cstring();
            int            read_binary(char* outbuf, int maxlen);
            
            bool copy(const void* inbuf, int len);
            void begin(short cmd, char ver = c_default_version, char subver = c_default_subversion);
            void end();
            
            /* for parser to build packet */
            bool append(const char* inbuf, int len)
            { return codec::write_b(inbuf, len); }
        };
        
    }
}

#endif

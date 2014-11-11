//
//  codec.cpp
//  console
//
//  Created by huanghaifeng on 14-10-1.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "codec.h"

namespace frog
{
    namespace generic
    {
        /* header: | flag_2 | cmd_2 | version_1 | subversion_1 | bodylen_2 | checkcode_1 | */
        
        short codec::command()
        {
            short cmd;
            read_header_b((char*)&cmd, sizeof(short), 2);
            return utils::endian_op(cmd);
        }
        
        char codec::version()
        {
            char v;
            read_header_b(&v, sizeof(char), 4);
            return utils::endian_op(v);
        }
        
        char codec::subversion()
        {
            char sv;
            read_header_b(&sv, sizeof(char), 5);
            return utils::endian_op(sv);
        }
        
        short codec::body_length()
        {
            short bodylen;
            read_header_b((char*)&bodylen, sizeof(short), 6);
            return utils::endian_op(bodylen);
        }
        
        unsigned char codec::check_code()
        {
            unsigned char code;
            read_header_b((char*)&code, sizeof(unsigned char), 8);
            return utils::endian_op(code);
        }
        
        void codec::reset()
        {
            readptr_ = c_header_size;
            size_ = c_header_size;
        }
     
        bool codec::copy_b(const void* inbuf, int len)
        {
            if(len > c_buffer_size)
            { return false; }
            reset();
            memcpy(buffer_, inbuf, len);
            size_ = len;
            return true;
        }
        
        void codec::begin_b(short cmd, char ver, char subver)
        {
            reset();
            const char flag[] = "GP";
			cmd = utils::endian_op(cmd);
			ver = utils::endian_op(ver);
			subver = utils::endian_op(subver);
            write_header_b(flag, sizeof(char)*2, 0);
            write_header_b((char*)&cmd, sizeof(short), 2);
            write_header_b(&ver, sizeof(char), 4);
            write_header_b(&subver, sizeof(char), 5);
        }
        
        void codec::end_b()
        {
            short bodylen = static_cast<short>(size_ - c_header_size);
			bodylen = utils::endian_op(bodylen);
            write_header_b((char*)&bodylen, sizeof(short), 6);
            unsigned char code = 0; // TODO do verification
			code = utils::endian_op(code);
            write_header_b((char*)&code, sizeof(unsigned char), 8);
        }
        
        bool codec::read_b(char* outbuf, int len)
        {
            if((len + readptr_) > size_ || (len + readptr_) > c_buffer_size)
            { return false; }
            memcpy(outbuf, buffer_ + readptr_, len);
            readptr_ += len;
            return true;
        }
        
        bool codec::read_del_b(char* outbuf, int len)
        {
            if(!read_b(outbuf, len))
            { return false; }
            memmove(buffer_ + readptr_ - len, buffer_ + readptr_, size_ - readptr_);
            readptr_ -= len;
            size_ -= len;
            end_b();
            return true;
        }
        
        void codec::read_undo_b(int len)
        {
            readptr_ -= len;
        }
        
        char* codec::read_bytes_b(int len)
        {
            if((len + readptr_) > size_)
            { return NULL; }
            char* p = &buffer_[readptr_];
            readptr_ += len;
            return p;
            
        }
        
        bool codec::write_b(const char* inbuf, int len)
        {
            if((size_ < 0) || ((len + size_) > c_buffer_size))
            { return false; }
            memcpy(buffer_+size_, inbuf, len);
            size_ += len;
            return true;
        }
        
        bool codec::write_front_b(const char* inbuf, int len)
        {
            if((len + size_) > c_buffer_size)
            { return false; }
            memmove(buffer_ + c_header_size + len, buffer_ + c_header_size, size_ - c_header_size);
            memcpy(buffer_ + c_header_size, inbuf, len);
            size_ += len;
            end_b();
            return true;
        }
        
        bool codec::write_zero_b()
        {
            if((size_ + 1) > c_buffer_size)
            { return false; }
            memset(buffer_+size_, '\0', sizeof(char));
            ++size_;
            return true;
        }
        
        void codec::read_header_b(char* outbuf, int len, int pos)
        {
            if(pos > 0 || pos+len < c_header_size)
            { memcpy(outbuf, buffer_+pos, len); }
        }
        
        void codec::write_header_b(const char* inbuf, int len, int pos)
        {
            if(pos > 0 || pos+len < c_header_size)
            { memcpy(buffer_+pos, inbuf, len); }
        }
        
    }
}

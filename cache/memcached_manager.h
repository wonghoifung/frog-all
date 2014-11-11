//
//  memcached_manager.h
//  console
//
//  Created by huanghaifeng on 14-10-25.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef memcached_manager_header
#define memcached_manager_header

#include <map>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <libmemcached/memcached.hpp>
#include <libmemcached/exception.hpp>

namespace frog
{
    namespace cache
    {
        
        typedef boost::shared_ptr<memcache::Memcache> memcache_t;
        
        class memcached_manager : public boost::noncopyable
        {
        private:
            memcached_manager() {}
            
            ~memcached_manager() {}
            
        public:
            static memcached_manager& ref();
            
            bool add_cache(int tag, const std::string& ip, int port);
            
            bool del_cache(int tag);
            
            bool get(int tag,
                     const std::string&key,
                     std::string& value);
            
            bool set(int tag,
                     const std::string& key,
                     const std::string& value,
                     time_t dura);
            
            bool del(int tag, const std::string& key);
            
        private:
            std::map<int, memcache_t> caches_;
        };
        
    }
}

#endif 


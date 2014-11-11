//
//  memcached_manager.cpp
//  console
//
//  Created by huanghaifeng on 14-10-25.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "memcached_manager.h"
#include <boost/make_shared.hpp>

namespace frog
{
    namespace cache
    {
        
        memcached_manager& memcached_manager::ref()
        {
            static memcached_manager mgr;
            return mgr;
        }
        
        bool memcached_manager::add_cache(int tag, const std::string& ip, int port)
        {
            memcache_t newcache = boost::make_shared<memcache::Memcache>();
            if (newcache->addServer(ip, port))
            {
                del_cache(tag);
                return caches_.insert(std::make_pair(tag, newcache)).second;
            }
            return false;
        }
        
        bool memcached_manager::del_cache(int tag)
        {
            std::map<int, memcache_t>::iterator it(caches_.find(tag));
            if (it != caches_.end())
            {
                caches_.erase(it);
            }
            return true;
        }
        
        bool memcached_manager::get(int tag,
                                    const std::string&key,
                                    std::string& value)
        {
            bool ret = false;
            std::map<int, memcache_t>::iterator it(caches_.find(tag));
            if (it != caches_.end())
            {
                try
                {
                    std::vector<char> result;
                    ret = it->second->get(key, result);
                    value.assign(result.begin(),result.end());
                }
                catch (memcache::Exception& e)
                {
                    //printf("memcached_manager::get error:%s, errno:%d", e.what(), e.getErrno());
                }
            }
            return ret;
        }
        
        bool memcached_manager::set(int tag,
                                    const std::string& key,
                                    const std::string& value,
                                    time_t dura)
        {
            bool ret = false;
            std::map<int, memcache_t>::iterator it(caches_.find(tag));
            if (it != caches_.end())
            {
                try
                {
                    std::vector<char> input(value.begin(),value.end());
                    ret = it->second->set(key, input, dura, 0);
                }
                catch (memcache::Exception& e)
                {
                    //printf("memcached_manager::set error:%s, errno:%d", e.what(), e.getErrno());
                }
            }
            return ret;
        }
        
        bool memcached_manager::del(int tag, const std::string& key)
        {
            bool ret = false;
            std::map<int, memcache_t>::iterator it(caches_.find(tag));
            if (it != caches_.end())
            {
                try
                {
                    ret = it->second->remove(key);
                }
                catch (memcache::Exception& e)
                {
                    //printf("memcached_manager::del error:%s, errno:%d", e.what(), e.getErrno());
                }
            }
            return ret;
        }
        
    }
}


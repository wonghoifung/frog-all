//
//  hash_tag.h
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef hash_cache_header
#define hash_cache_header

#include <assert.h>
#include <set>
#include <boost/noncopyable.hpp>
#include "utils/global_holder.h"

namespace frog
{
    namespace utils
    {
        
        class hash_tag : public boost::noncopyable
        {
        public:
            hash_tag()
            {
                
            }
            
            ~hash_tag()
            {
                
            }
            
            void add_tag(int tag)
            {
                bool ret(tags_.insert(tag).second);
                assert(ret);
            }
            
            int get_tag(int num)
            {
                assert(tags_.size());
                int hash = num % tags_.size();
                for (std::set<int>::iterator it = tags_.begin(); it != tags_.end(); ++it)
                {
                    --hash;
                    if (hash < 0)
                    {
                        return *it;
                    }
                }
                assert(false);
                return -1;
            }
            
        private:
            std::set<int> tags_;
        };
        
    }
}

#define global_hash_tag frog::utils::global_holder<frog::utils::hash_tag>::ref()

#endif

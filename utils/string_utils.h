//
//  string_utils.h
//  console
//
//  Created by huanghaifeng on 14-10-26.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef string_utils_header
#define string_utils_header

#include <sstream>
#include <string>

namespace frog
{
    namespace utils
    {
        
        template <typename T>
        inline T string_to_number(const std::string& str)
        {
            std::stringstream ss(str);
            T result;
            ss >> result;
            return result;
        }
        
    }
}

#endif

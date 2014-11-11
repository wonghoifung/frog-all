//
//  chat_command.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef chat_command_header
#define chat_command_header

namespace frog
{
    namespace chat
    {
        
        /* range [1,1000] is private */
        
        enum
        {
            cmd_user_register = 1,
            cmd_server_register = 2,
            cmd_broadcast_user_login = 8,
            cmd_broadcast_user_logout = 9,
        };
        
        enum
        {
            cmd_user_broadcast = 1001,
        };
        
    }
}

#endif

//
//  proxy_command.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef proxy_command_header
#define proxy_command_header

namespace frog
{
    namespace proxy
    {
        
        /* range [1,1000] is private */
        
        enum
        {
            cmd_user_register = 1,
            cmd_server_register = 2,
            cmd_force_quit = 3,
            cmd_server_to_server_request = 4,
            cmd_server_to_server_response = 5,
            cmd_server_to_user = 6,
            cmd_no_service_found = 7,
            cmd_broadcast_user_login = 8,
            cmd_broadcast_user_logout = 9,
        };
        
    }
}

#endif

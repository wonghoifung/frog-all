//
//  mail_command.h
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef mail_command_header
#define mail_command_header

namespace frog
{
    namespace mail
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
            cmd_user_get_all_mail_brief = 2001,
            cmd_user_send_mail = 2002,
            cmd_user_delete_mail = 2003,
            cmd_user_get_mail_detail = 2004,
            cmd_user_mail_alert = 2005,
        };
        
    }
}

#endif

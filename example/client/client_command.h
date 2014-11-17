#ifndef CLIENT_COMMAND_HEADER
#define CLIENT_COMMAND_HEADER

enum
{
    cmd_user_register = 1,
    cmd_force_quit = 3,
    cmd_server_to_user = 6,
    cmd_no_service_found = 7,
};

/* chat */
enum
{
    cmd_user_broadcast = 1001,
};

/* mail */
enum
{
    cmd_user_get_all_mail_brief = 2001,
    cmd_user_send_mail = 2002,
    cmd_user_delete_mail = 2003,
    cmd_user_get_mail_detail = 2004,
    cmd_user_mail_alert = 2005,
};

#endif


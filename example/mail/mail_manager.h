//
//  mail_manager.h
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef mail_manager_header
#define mail_manager_header

#include <boost/noncopyable.hpp>
#include "generic/tcpserver.h"

namespace frog
{
    namespace mail
    {
        
        class mail_manager : public boost::noncopyable
        {
            struct mail_brief
            {
                int mailid;
                int from;
                std::string title;
                int sendts;
                int attachment;
                int flag;
            };
            
        public:
            mail_manager();
            ~mail_manager();
            void lcmd_mail_stat_more(std::vector<std::string>&,generic::tcpsession_ptr session);
            void dispatch(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack);
            
        private:
            void handle_get_all_mail_brief(frog::generic::tcpsession_ptr session,
                                           frog::generic::decoder* pack);
            void handle_send_mail(frog::generic::tcpsession_ptr session,
                                  frog::generic::decoder* pack);
            void handle_delete_mail(frog::generic::tcpsession_ptr session,
                                    frog::generic::decoder* pack);
            void handle_get_mail_detail(frog::generic::tcpsession_ptr session,
                                        frog::generic::decoder* pack);
            
        private:
            std::string get_table_name(int userid);
            bool get_all_mail_brief(int userid, std::vector<mail_brief>& mailbriefs);
            bool get_all_mail_brief_from_cache(int userid, std::vector<mail_brief>& mailbriefs);
            bool set_all_mail_brief_to_cache(int userid, std::vector<mail_brief>& mailbriefs);
            bool get_all_mail_brief_from_db(int userid, std::vector<mail_brief>& mailbriefs);
        };

    }
}

#endif 


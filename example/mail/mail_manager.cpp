//
//  mail_manager.cpp
//  mail
//
//  Created by huanghaifeng on 14-11-15.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "mail_manager.h"
#include "logger.h"
#include "config.h"
#include "hash_tag.h"
#include "mail_command.h"
#include "memcached_manager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#define key_all_mail_brief ("allmailbrief")
//#define key_mail_detail ("maildetail")

namespace frog
{
    namespace mail
    {
        
        mail_manager::mail_manager()
        {
            
        }
        
        mail_manager::~mail_manager()
        {
            
        }
        
        void mail_manager::lcmd_mail_stat_more(std::vector<std::string>&,generic::tcpsession_ptr session)
        {
            std::stringstream ss;
            ss << "under construction..." << std::endl;
            session->send_line(ss.str().c_str(), (int)ss.str().size());
        }
        
        void mail_manager::dispatch(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
        {
            int cmd = pack->command();
            switch (cmd) {
                case cmd_user_get_all_mail_brief:
                    handle_get_all_mail_brief(session, pack);
                    break;
                    
                case cmd_user_send_mail:
                    handle_send_mail(session, pack);
                    break;
                    
                case cmd_user_delete_mail:
                    handle_delete_mail(session, pack);
                    break;
                    
                case cmd_user_get_mail_detail:
                    handle_get_mail_detail(session, pack);
                    break;
                    
                default:
                    break;
            }
        }
        
        void mail_manager::handle_get_all_mail_brief(frog::generic::tcpsession_ptr session,
                                                     frog::generic::decoder* pack)
        {
            int userid = pack->read_int();
            std::vector<mail_brief> mbriefs;
            if (get_all_mail_brief(userid,mbriefs))
            {
                frog::generic::encoder op;
                op.begin(cmd_user_get_all_mail_brief);
		op.write_int(userid);
                op.write_short(0); /* success */
                op.write_int((int)(mbriefs.size()));
                for (int i=0; i<mbriefs.size(); ++i)
                {
                    op.write_int(mbriefs[i].mailid);
                    op.write_int(mbriefs[i].from);
                    op.write_string(mbriefs[i].title);
                    op.write_int(mbriefs[i].sendts);
                    op.write_int(mbriefs[i].attachment);
                    op.write_int(mbriefs[i].flag);
                }
                op.end();
                session->send(&op);
                return;
            }
            
            frog::generic::encoder op;
            op.begin(cmd_user_get_all_mail_brief);
	    op.write_int(userid);
            op.write_short(1); /* failure */
            op.end();
            session->send(&op);
        }
        
        void mail_manager::handle_send_mail(frog::generic::tcpsession_ptr session,
                                            frog::generic::decoder* pack)
        {
            int userid = pack->read_int();
            int to = pack->read_int();
            std::string table(get_table_name(to));
            std::string title = pack->read_cstring();
            std::string content = pack->read_cstring();
            std::string attachment = pack->read_cstring();
            if (!global_dboperator.get().execute(
                "insert into %s(`from`,`to`,ntitle,contents,attachment,flag,sendts) "
                "values(%d,%d,'%s','%s','%s',0,UNIX_TIMESTAMP());",table.c_str(),
                userid,to,title.c_str(),content.c_str(),attachment.c_str()))
            {
                logstr("insert failure, userid:%d, to:%d, title:%s",userid,to,title.c_str());
                frog::generic::encoder op;
                op.begin(cmd_user_send_mail);
                op.write_int(userid);
                op.write_short(1); /* failure */
                op.end();
                session->send(&op);
                return;
            }
            frog::generic::encoder op;
            op.begin(cmd_user_send_mail);
            op.write_int(userid);
            op.write_short(0); /* success */
            op.end();
            session->send(&op);
            {
                /* notify receiver */
                frog::generic::encoder op;
                op.begin(cmd_user_mail_alert);
                op.write_int(to);
                op.write_int(userid);
                op.write_string(title);
                op.end();
                session->send(&op);
            }
            
            /* safe but not efficient way to refresh cache */
            std::vector<mail_brief> mbriefs;
            if (get_all_mail_brief_from_db(to, mbriefs))
            {
                set_all_mail_brief_to_cache(to, mbriefs);
            }
        }
        
        void mail_manager::handle_delete_mail(frog::generic::tcpsession_ptr session,
                                              frog::generic::decoder* pack)
        {
            int userid = pack->read_int();
            int mailid = pack->read_int();
            std::string table(get_table_name(userid));
            if (!global_dboperator.get().execute("delete from %s where `id`=%d",table.c_str(),mailid))
            {
                logstr("delete failure, userid:%d, mailid:%d",userid,mailid);
                frog::generic::encoder op;
                op.begin(cmd_user_delete_mail);
                op.write_int(userid);
                op.write_short(1); /* failure */
                op.end();
                session->send(&op);
                return;
            }
            frog::generic::encoder op;
            op.begin(cmd_user_delete_mail);
            op.write_int(userid);
            op.write_short(0); /* success */
            op.end();
            session->send(&op);
            
            /* refresh cache */
            std::vector<mail_brief> mbriefs;
            if (get_all_mail_brief(userid, mbriefs))
            {
                std::vector<mail_brief>::iterator beg(mbriefs.begin()),end(mbriefs.end());
                for (; beg!=end; ++beg)
                {
                    if (beg->mailid == mailid)
                    {
                        mbriefs.erase(beg);
                        set_all_mail_brief_to_cache(userid, mbriefs);
                        return;
                    }
                }
            }
        }
        
        void mail_manager::handle_get_mail_detail(frog::generic::tcpsession_ptr session,
                                                  frog::generic::decoder* pack)
        {
            int userid = pack->read_int();
            int mailid = pack->read_int();
            std::string table(get_table_name(userid));
            frog::db::dataset* ds = global_dboperator.get().query("select * from %s where `id`=%d",
                                                                  table.c_str(),mailid);
            if (!ds || ds->row_count() < 1)
            {
                logstr("select failure, userid:%d, mailid:%d",userid,mailid);
                frog::generic::encoder op;
                op.begin(cmd_user_get_mail_detail);
                op.write_int(userid);
                op.write_short(1); /* failure */
                op.end();
                session->send(&op);
                return;
            }
            
            int from = ds->get_field_int32(0, "from");
            std::string title = ds->get_field_string(0, "ntitle");
            std::string contents = ds->get_field_string(0, "contents");
            std::string attachment = ds->get_field_string(0, "attachment");
            int flag = ds->get_field_int32(0, "flag");
            int sendts = ds->get_field_int32(0, "sendts");
            
            frog::generic::encoder op;
            op.begin(cmd_user_get_mail_detail);
            op.write_int(userid);
            op.write_short(0); /* success */
            op.write_int(from);
            op.write_string(title);
            op.write_string(contents);
            op.write_string(attachment);
            op.write_int(sendts);
            op.end();
            session->send(&op);
            
            /* refresh cache */
            if (!flag)
            {
                /* already read, write db */
                if (!global_dboperator.get().execute("update %s set flag=1 where `id`=%d",
                                                     table.c_str(),mailid))
                {
                    logstr("update failure, userid:%d, mailid:%d",userid,mailid);
                    return;
                }
                /* already read, write cache */
                std::vector<mail_brief> mbriefs;
                if (get_all_mail_brief_from_cache(userid, mbriefs))
                {
                    for (int i=0; i<mbriefs.size(); ++i)
                    {
                        if (mbriefs[i].mailid == mailid)
                        {
                            mbriefs[i].flag = 1;
                            set_all_mail_brief_to_cache(userid, mbriefs);
                            return;
                        }
                    }
                }
            }
        }
        
        std::string mail_manager::get_table_name(int userid)
        {
            int mailtable_idx = userid % config::ref().get_mailtable_count();
            std::stringstream ss;
            ss << config::ref().get_mailtable_db() << "." <<
                  config::ref().get_mailtable_name() << mailtable_idx;
            return ss.str();
        }
        
        bool mail_manager::get_all_mail_brief(int userid, std::vector<mail_brief>& mailbriefs)
        {
            /* try cache first */
            if (get_all_mail_brief_from_cache(userid, mailbriefs))
            {
                return true;
            }
            /* try db if found nothing in cache */
            if (get_all_mail_brief_from_db(userid, mailbriefs))
            {
                set_all_mail_brief_to_cache(userid, mailbriefs);
                return true;
            }
            return false;
        }
        
        bool mail_manager::get_all_mail_brief_from_cache(int userid,
                                                         std::vector<mail_brief>& mailbriefs)
        {
            mailbriefs.clear();
            using namespace boost::property_tree;
            int cache_tag = global_hash_tag.get().get_tag(userid);
            std::stringstream key; key << key_all_mail_brief << userid;
            std::string value;
            if (frog::cache::memcached_manager::ref().get(cache_tag, key.str(), value))
            {
                std::stringstream ss(value);
                ptree pt;
                try
                {
                    read_json(ss, pt);
                    BOOST_FOREACH(boost::property_tree::ptree::value_type& v, pt)
                    {
                        mail_brief mbrief;
                        mbrief.mailid = v.second.get<int>("id");
                        mbrief.from = v.second.get<int>("from");
                        mbrief.title = v.second.get<std::string>("ntitle");
                        mbrief.sendts = v.second.get<int>("sendts");
                        mbrief.attachment = v.second.get<int>("attachment");
                        mbrief.flag = v.second.get<int>("flag");
                        mailbriefs.push_back(mbrief);
                    }
                    return true;
                }
                catch(ptree_error& e)
                {
                    logstr(e.what());
                }
            }
            return false;
        }
        
        bool mail_manager::set_all_mail_brief_to_cache(int userid,
                                                       std::vector<mail_brief>& mailbriefs)
        {
            using namespace boost::property_tree;
            try
            {
                ptree array;
                for (int i=0; i<mailbriefs.size(); ++i)
                {
                    ptree node;
                    node.put("id", mailbriefs[i].mailid);
                    node.put("from", mailbriefs[i].from);
                    node.put("ntitle", mailbriefs[i].title);
                    node.put("sendts", mailbriefs[i].sendts);
                    node.put("attachment", mailbriefs[i].attachment);
                    node.put("flag", mailbriefs[i].flag);
                    array.push_back(std::make_pair("", node));
                }
                
                int cache_tag = global_hash_tag.get().get_tag(userid);
                std::stringstream key; key << key_all_mail_brief << userid;
                std::stringstream value;
                write_json(value, array);
                
                if (frog::cache::memcached_manager::ref().set(cache_tag, key.str(), value.str(), 0))
                {
                    logstr("set key(%s) failed", key.str().c_str());
                    return false;
                }
                return true;
            }
            catch(ptree_error& e)
            {
                logstr(e.what());
            }
            return false;
        }
        
        bool mail_manager::get_all_mail_brief_from_db(int userid,
                                                      std::vector<mail_brief>& mailbriefs)
        {
            mailbriefs.clear();
            std::string table(get_table_name(userid));
            frog::db::dataset* ds = global_dboperator.get().query("select * from %s where `to`=%d;",
                                                                  table.c_str(), userid);
            if (!ds)
            {
                logstr("select %s error");
                return false;
            }
            
            for (int i=0; i<ds->row_count(); ++i)
            {
                mail_brief mbrief;
                mbrief.mailid = ds->get_field_int32(i, "id");
                mbrief.from = ds->get_field_int32(i, "from");
                mbrief.title = ds->get_field_string(i, "ntitle");
                mbrief.sendts = ds->get_field_int32(i, "sendts");
                std::string attachment = ds->get_field_string(i, "attachment");
                mbrief.attachment = !attachment.empty();
                mbrief.flag = ds->get_field_int32(i, "flag");
                mailbriefs.push_back(mbrief);
            }
            return true;
        }
        
    }
}


//
//  proxy_client_handler.cpp
//  test_client
//
//  Created by huanghaifeng on 14-10-16.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "proxy_client_handler.h"
#include "console_server_handler.h"

namespace frog
{
	namespace generic
	{

		proxy_client_handler::proxy_client_handler(int userid):userid_(userid),connected_(false),registered_(false),console_(NULL)
		{
		}

		void proxy_client_handler::installcb(tcpclient& client)
		{
			client.set_messagecb(boost::bind(&proxy_client_handler::on_message,this,_1,_2));
			client.set_closecb(boost::bind(&proxy_client_handler::on_close,this,_1));
			client.set_connectcb(boost::bind(&proxy_client_handler::on_connect,this,_1));
			client.set_errorcb(boost::bind(&proxy_client_handler::on_error,this,_1,_2));
		}

		int proxy_client_handler::on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int cmd = pack->command();
			switch (cmd) 
			{
				case cmd_user_register:
					handle_register(session, pack);
					break;

				case cmd_force_quit:
					handle_force_quit(session, pack);
					break;

				case cmd_server_to_user:
					handle_server_to_user(session, pack);
					break;

				case cmd_no_service_found:
					handle_no_service_found(session, pack);
					break;

				case cmd_user_broadcast:
					handle_user_broadcast(session, pack);
					break;

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

				case cmd_user_mail_alert:
					handle_mail_alert(session, pack);
					break;

				default:
					{
						std::stringstream ss;
						ss << "unknown command: " << cmd << std::endl;	
						console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
						console_->linesession()->send_prompt();
					}
					break;
			}

			return 0;
		}

		void proxy_client_handler::on_close(frog::generic::tcpsession_ptr session)
		{
			clear_connect_state();
			std::cout << userid_ << ":" << session->localaddr() << " - " << session->remoteaddr() << " closed" << std::endl;
		}

		void proxy_client_handler::on_connect(frog::generic::tcpsession_ptr session)
		{
			if (session)
			{
				std::cout << session->localaddr() << " - " << session->remoteaddr() << " connected" << std::endl;
				connected_ = true;
				session_ = session;
			}
			else
			{
				std::cout << userid_ << ": cannot connect" << std::endl;
			}
		}

		void proxy_client_handler::on_error(frog::generic::tcpsession_ptr session, int errcode)
		{
			clear_connect_state();
			std::cout << userid_ << ":" << session->localaddr() << " - "  << session->remoteaddr() << " error:" << errcode << std::endl;
		}

		void proxy_client_handler::handle_register(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int userid = pack->read_int();
			int ret = pack->read_int();
			if (ret == 0)
			{
				registered_ = true;
				std::stringstream ss;
				ss << "login success, userid:" << userid << std::endl;
				console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
				console_->linesession()->send_prompt();
			}
			else
			{
				std::stringstream ss;
				ss << "login failure, userid:" << userid << ", ret:" << ret << std::endl;
				console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
				console_->linesession()->send_prompt();
			}
		}

		void proxy_client_handler::handle_force_quit(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int reason = pack->read_int();
			std::stringstream ss;
			ss << "force quit, userid:" << userid_ << ", reason:" << reason << std::endl;
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::handle_server_to_user(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int tag = pack->read_int(); 
			int subcmd = pack->read_int();
			std::stringstream ss;
			ss << "server to user, userid:" << userid_ << ",tag:" << tag << std::endl;
			if(subcmd==0)
			{
				std::string serverbroadcast = pack->read_cstring();
				ss << "server broadcast: " << serverbroadcast << std::endl;
			}	
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::handle_no_service_found(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			std::stringstream ss;
			ss << "no service found, userid:" << userid_ << std::endl;
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::handle_user_broadcast(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int tag = pack->read_int();
			int userid = pack->read_int();
			std::string statement = pack->read_cstring();

			std::stringstream ss;
			ss << "this is " << userid_ << std::endl;
			ss << "tag: " << tag << std::endl;
			ss << userid << " say: " << statement << std::endl;
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::handle_get_all_mail_brief(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int userid = pack->read_int();
			(void)userid;
			int ret = pack->read_short();
			if(ret == 0) // success
			{
				std::stringstream ss;
				ss << std::endl;
				ss << "this is " << userid_ << std::endl;
				int size = pack->read_int();
				for(int i=0; i<size; ++i)
				{
					int mailid = pack->read_int();
					int from = pack->read_int();
					std::string title = pack->read_cstring();
					int sendts = pack->read_int();
					int attachment = pack->read_int();
					int flag = pack->read_int();
					ss << "mailid:" << mailid 
					   << ",from:" << from 
					   << ",title:" << title 
					   << ",sendts:" << sendts 
					   << ",atta:" << attachment
					   << ",flag:" << flag << std::endl;
				}
				ss << std::endl;
				console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
				console_->linesession()->send_prompt();
			}
			else if(ret == 1)
			{
				std::stringstream ss;
				ss << "this is " << userid_ << std::endl;
				ss << "cannot get all mail brief" << std::endl;
				console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
				console_->linesession()->send_prompt();
			}
			else
			{
				std::stringstream ss;
				ss << "this is " << userid_ << std::endl;
				ss << "cannot get all mail brief, unknown ret:" << ret << std::endl;
				console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
				console_->linesession()->send_prompt();
			}
		}

		void proxy_client_handler::handle_send_mail(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int userid = pack->read_int();
			(void)userid;
			int ret = pack->read_short();
			std::stringstream ss;
			ss << "this is " << userid_ << std::endl;
			ss << "send mail " << (ret==0?"success, ret:":"failure, ret:") << ret << std::endl;
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::handle_delete_mail(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int userid = pack->read_int();
			(void)userid;
			int ret = pack->read_short();
			std::stringstream ss;
			ss << "this is " << userid_ << std::endl;
			ss << "delete mail " << (ret==0?"success, ret:":"failure, ret:") << ret << std::endl;
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::handle_get_mail_detail(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int userid = pack->read_int();
			(void)userid;
			int ret = pack->read_short();
			std::stringstream ss;
			ss << "this is " << userid_ << std::endl;
			ss << "get mail detail " << (ret==0?"success, ret:":"failure, ret:") << ret << std::endl;
			if(ret==0)
			{
				int from = pack->read_int();
				std::string title = pack->read_cstring();
				std::string contents = pack->read_cstring();
				std::string attachment = pack->read_cstring();
				int sendts = pack->read_int();
				ss << "from:" << from << std::endl;
				ss << "title:" << title << std::endl;
				ss << "contents:" << contents << std::endl;
				ss << "attachment:" << attachment << std::endl;
				ss << "sendts:" << sendts << std::endl;
			}
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::handle_mail_alert(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack)
		{
			int to = pack->read_int();
			int from = pack->read_int();
			std::string title = pack->read_cstring();
			std::stringstream ss;
			ss << "this is " << userid_ << std::endl;
			ss << "mail alert, from:" << from << ", to:" << to << ", title:" << title << std::endl;
			console_->linesession()->send_line(ss.str().c_str(), (int)ss.str().size());
			console_->linesession()->send_prompt();
		}

		void proxy_client_handler::clear_connect_state()
		{
			connected_ = false;
			session_ = frog::generic::tcpsession_ptr();
			registered_ = false;
		}

	}
}



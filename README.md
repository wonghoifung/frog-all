frog-all
=========

A nonblocking single-threaded network message dispatcher based on Boost.Asio

1. very easy to use
2. console to inspect running status
3. tcp session heartbeat checking
4. cache manager
5. db manager

quick glance(full source code could be found in unittest folder)
================================================================

        class generic_server_handler : public boost::noncopyable
        {
        public:
            void installcb(tcpserver& s)
            {
                /* register all callbacks */
                s.set_messagecb(boost::bind(&generic_server_handler::on_message,this,_1,_2));
                s.set_closecb(boost::bind(&generic_server_handler::on_close,this,_1));
                s.set_connectcb(boost::bind(&generic_server_handler::on_connect,this,_1));
                s.set_timeoutcb(boost::bind(&generic_server_handler::on_timeout,this,_1));
                s.set_errorcb(boost::bind(&generic_server_handler::on_error,this,_1,_2));
            }
            int on_message(frog::generic::tcpsession_ptr session, frog::generic::decoder* pack) {  return 0; }
            void on_close(frog::generic::tcpsession_ptr session) {}
            void on_connect(frog::generic::tcpsession_ptr session) {}
            void on_timeout(frog::generic::tcpsession_ptr session) {}
            void on_error(frog::generic::tcpsession_ptr session, int errcode) {}
        };
        
        
        int main(int argc, char* argv[])
        {
          try 
          {    
            boost::asio::io_service io_service;
            frog::utils::global_holder<boost::asio::io_service>::ref().set(&io_service);
        
            /* init main logic server */
            frog::generic::tcpserver gserver(io_service, 9878, frog::generic::parse_pack);
            frog::generic::generic_server_handler ghandler;
            ghandler.installcb(gserver);
            gserver.run();
        
            /* init the console server for afterwards inspection */
            frog::generic::tcpserver consoleserver(io_service, 9877, frog::generic::parse_line);
            /* FIXME use different handler class */
            frog::generic::generic_server_handler consolehandler;
            consolehandler.installcb(consoleserver);
            consoleserver.run();
        
            io_service.run();
          } 
          catch (std::exception& e) 
          {
            std::cerr << "Exception: " << e.what() << "\n";
          }
        }
        
how to use console
==================

When server is up, telnet console server in command line. like:

$ telnet 127.0.0.1 9877

\> help


frog-all location on my linux
==============================

[wonghoifung@local100 frog-all]$ ll ..

total 122652

drwxrwxr-x 11 wonghoifung devel     4096 Oct 15 11:42 boost_1_53_0

drwxrwxr-x 10 wonghoifung devel     4096 Nov 17 17:44 frog-all

drwxrwxr-x  5 wonghoifung devel     4096 Nov 10 10:39 mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit

drwxrwxr-x 10 wonghoifung devel     4096 Nov 10 11:07 postgresql-9.3.5


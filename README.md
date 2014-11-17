frog-all
=========

A nonblocking single-threaded network framework based on Boost.Asio

frog-all is very easy to use, it is designed to build network programme in a fairly short time and in a really convenient way.

frog-all provides a console for developers to inspect programme stat in runtime. 

frog-all provides tcp session heartbeat checking.

frog-all uses simple xdr instead of protobuf, it looks fresher and more flexable to manual control serialization when we don't need anything more complicated.

frog-all probably is cross-platform because it depends on Boost to handle IO issues, it has been tested on windows/mac os x/linux.

frog-all was inspired by my work experiences, as a server programmer, I was always asked to do my jobs effectively and respond to bugs quickly, so the code is much better to be wrote as simple as possible provided the performance is up to standard.

Stress test case for the example server can be found in frog-erlang repos. Erlang is suitable to do stress testing because it can spawn thousands of processes easily and cost very little resouce.

quick glance(full source code could be found in example folder)
===============================================================

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



#include "generic_server_handler.h"
#include "utils/global_holder.h"
#include "utils/pid_file.h"
#include "generic/udpserver.h"

void on_dgram(char* data,int len,frog::generic::udpserver& usrv)
{
    std::string s(data,len);
    std::cout << usrv.localaddr() << " - " << usrv.remoteaddr() << " : " << s << std::endl;
    usrv.sendback(data, len);
}

int main(int argc, char* argv[])
{
    frog::utils::create_pid_file();
    try 
    {    
        boost::asio::io_service io_service;
        frog::utils::global_holder<boost::asio::io_service>::ref().set(&io_service);
        
        frog::generic::tcpserver gserver(io_service, 9878, frog::generic::parse_pack);
        frog::generic::generic_server_handler ghandler;
        ghandler.installcb(gserver);
        gserver.run();
        
	frog::generic::tcpserver consoleserver(io_service, 9877, frog::generic::parse_line);
	/* FIXME should use different handler class */
        frog::generic::generic_server_handler consolehandler;
        consolehandler.installcb(consoleserver);
        consoleserver.run();
        
        frog::generic::udpserver userver(io_service, 9879);
        userver.set_dgramcb(on_dgram);
        userver.run();
        
	io_service.run();
    } 
    catch (std::exception& e) 
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

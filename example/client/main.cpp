#include "console_server_handler.h"
#include "logger.h"

int main(int argc, char* argv[])
{
    if (!frog::utils::logger::ref().init("client", "./"))
    {
        std::cerr << "init logger error\n";
        return -1;
    }

    try
    {
        boost::asio::io_service io_service;
        frog::utils::global_holder<boost::asio::io_service>::ref().set(&io_service);

	frog::generic::tcpserver consoleserver(io_service,12345,frog::generic::parse_line);
        frog::console::console_server_handler chandler;
        chandler.installcb(consoleserver);
        consoleserver.run();
	
	user gu;
	global_user.set(&gu);
	global_user.get().id = 1;

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}



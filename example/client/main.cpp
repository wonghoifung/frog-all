#include "generic_client_handler.h"
#include "generic/udpclient.h"
#include "chat_client_handler.h"

void on_dgram_back(char* data, int len, frog::generic::udpclient& uclient)
{
    std::string s(data,len);
    std::cout << uclient.localaddr() << " - " << uclient.remoteaddr() << " : " << s << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "when server is ready, press enter." << std::endl;
    std::cin.get();
    try
    {
        boost::asio::io_service io_service;
        frog::utils::global_holder<boost::asio::io_service>::ref().set(&io_service);

	frog::generic::tcpclient gclient(io_service);
        frog::chat::chat_client_handler generichandler;
        generichandler.installcb(gclient);
        gclient.connect("127.0.0.1", "8888");
        /* 
        frog::generic::tcpclient gclient(io_service);
        frog::generic::generic_client_handler generichandler;
        generichandler.installcb(gclient);
        gclient.connect("127.0.0.1", "9878");
         
        frog::generic::udpclient uclient(io_service,"127.0.0.1", "9879");
        uclient.set_client_dgramcb(on_dgram_back);
        char request[1024] = "hello world";
        uclient.send(request, strlen(request));
        */
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}



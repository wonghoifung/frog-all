#include <iostream>
#include "config.h"

#if 0

int  main()
{
	//const char* cfgfile = "D:/UserProfiles/AlbertHuang/Desktop/frog/example/frog_cfg/frog_cfg.xml";
    const char* cfgfile = "/Users/huanghaifeng/Desktop/frog/config/frog_cfg.xml";
    
    frog::utils::config& globalcfg = frog::utils::config::ref();
    if (globalcfg.init(cfgfile))
    {
        std::cout << "port: " << globalcfg.get_port() << std::endl;
        std::cout << "console port: " << globalcfg.get_console_port() << std::endl;
        std::cout << "ip: " << globalcfg.get_ip() << std::endl;
        std::cout << "name: " << globalcfg.get_name() << std::endl;
        std::vector<std::string>& urls = globalcfg.get_urls();
        for (int i=0; i<urls.size(); ++i)
        {
            std::cout << i << " url: " << urls[i] << std::endl;
        }
        
        std::cout << "-----------------------" << std::endl;
        
        std::cout << "img id: " << globalcfg.get_imgid() << std::endl;
        std::vector<std::string>& imgs = globalcfg.get_imgs();
        for (int i=0; i<imgs.size(); ++i)
        {
            std::cout << i << " img: " << imgs[i] << std::endl;
        }
        
        std::cout << "-----------------------" << std::endl;
        
        std::cout << "new imgs: " << globalcfg.get_newimgs() << std::endl;
    }
    
	getchar();

    return 0;
}

#endif

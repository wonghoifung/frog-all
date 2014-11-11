#ifndef PID_FILE_HEADER
#define PID_FILE_HEADER

#include <fstream>
//#ifdef _WIN32
#if defined(_WIN32) || defined(__APPLE__) 
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#else
#include <unistd.h>
#endif

namespace frog
{
    namespace utils
    {

        void create_pid_file()
        {
//#ifdef _WIN32
#if defined(_WIN32) || defined(__APPLE__) 
            std::string fullpath = boost::filesystem::initial_path<boost::filesystem::path>().string();
            std::fstream pid;
            pid.open("./frog.pid", std::fstream::trunc | std::fstream::out);
            pid<< fullpath << "  " << getpid();
            pid.close();
#else
            std::fstream pid;
            pid.open("./frog.pid", std::fstream::trunc | std::fstream::out);
            pid<< get_current_dir_name() << "  " << getpid();
            pid.close();
#endif
        }

    }
}

#endif


#ifndef MYSQLWRAPPER_HEADER
#define MYSQLWRAPPER_HEADER

#ifdef WIN32
#include <process.h>
#include <winsock2.h>
#else
#include <sys/errno.h>
#include <pthread.h>
#endif

#include <mysql.h>
#include <mysqld_error.h>
#include "idbwrapper.h"

namespace frog
{
	namespace db
	{

		class mysqlwrapper : public boost::noncopyable, public idbwrapper
		{
		public:
			mysqlwrapper();
			virtual ~mysqlwrapper();
            
            virtual bool connect(const std::string& host,
                                 const std::string& user,
                                 const std::string& password,
                                 const std::string& dbname,
                                 unsigned int port,
                                 const std::string& unix_socket,
                                 const std::string& character);
			virtual bool disconnect();
            
			virtual dataset& get_dataset();
			virtual bool rollback();
			virtual bool commit();
			virtual bool start_transaction();
			virtual bool stop_transaction();
			virtual bool execute(const std::string& sql);
			virtual bool query(const std::string& sql);
			virtual unsigned long affected_rowcount();

		private:
			void clear();

		private:
			MYSQL* db_;
			MYSQL_RES* resultset_;
			dataset dataset_;
			unsigned long affected_rowcount_;
		};

	}
}

#endif


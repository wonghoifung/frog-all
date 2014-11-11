#ifndef IDBWRAPPER_HEADER
#define IDBWRAPPER_HEADER

#include <string>
#include <dataset.h>

#define SQLBUF_MAXLEN 2048

namespace frog
{
	namespace db
	{

		class idbwrapper
		{
		public:
			idbwrapper(){}

			virtual ~idbwrapper(){}

            virtual bool connect(const std::string& host,
                                 const std::string& user,
                                 const std::string& password,
                                 const std::string& dbname,
                                 unsigned int port,
                                 const std::string& unix_socket,
                                 const std::string& character) = 0;
            
			virtual bool disconnect() = 0;
            
			virtual dataset& get_dataset() = 0;

			virtual bool rollback() = 0;

			virtual bool commit() = 0;

			virtual bool start_transaction() = 0;

			virtual bool stop_transaction() = 0;

			virtual bool execute(const std::string& sql) = 0;

			virtual bool query(const std::string& sql) = 0;

			virtual unsigned long affected_rowcount() = 0;
		};

	}
}

#endif

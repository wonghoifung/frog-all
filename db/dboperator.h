#ifndef DBOPERATOR_HEADER
#define DBOPERATOR_HEADER

#include <string>
#include <cstdarg>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include "dbfactory.h"

namespace frog
{
	namespace db
	{

		struct conn_args_t
		{
			std::string host;
			std::string port;
			std::string user;
			std::string passwd;
			std::string db;
		};

		class dboperator : public boost::noncopyable
		{
		public:
			dboperator(dbtype dbt);
			~dboperator();
			bool connect(const conn_args_t& config);
			void disconnect();
			dataset* query(const char* format, ...);
			bool execute(const char* format, ...);
			bool start_transaction();
			bool stop_transaction();
			bool commit();
			bool rollback();
			
		private:
            const dbtype dbtype_;
			idbwrapper* dbwrapper_;
		};

	}
}

#endif


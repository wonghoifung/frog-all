#include "dboperator.h"

namespace frog
{
	namespace db
	{

		dboperator::dboperator(dbtype dbt):dbtype_(dbt),dbwrapper_(NULL)
		{ }

		dboperator::~dboperator() 
		{ disconnect(); }

		bool dboperator::connect(const conn_args_t& config)
		{
			dbwrapper_ = dbfactory::create_dbwrapper(dbtype_);
			char unixsock[32] = {0};
            return dbwrapper_->connect(config.host,
                                       config.user,
                                       config.passwd,
                                       config.db,
                                       atoi(config.port.c_str()),
                                       unixsock,
                                       "utf8");
		}

		void dboperator::disconnect()
		{
			if(dbwrapper_)
			{
                dbwrapper_->disconnect();
				dbfactory::dispose_dbwrapper(&dbwrapper_);
			}
		}

		dataset* dboperator::query(const char* format, ...)
		{
			char buf[SQLBUF_MAXLEN];
			va_list ap;
			va_start(ap, format);
			int ret = vsnprintf(buf, SQLBUF_MAXLEN, format, ap);
			va_end(ap);

			if( -1 == ret )
			{ return NULL; }

			if(dbwrapper_->query(buf) == false)
			{ return NULL; }
	
			return &(dbwrapper_->get_dataset());
		}

		bool dboperator::execute(const char* format, ...)
		{
			char buf[SQLBUF_MAXLEN] = {0};
			va_list ap;
			va_start(ap, format);
			int ret = vsnprintf( buf, SQLBUF_MAXLEN, format, ap);
			va_end(ap);

			if( -1 == ret)
			{ return false; }

			return dbwrapper_->execute(buf);
		}

		bool dboperator::start_transaction()
		{
			return dbwrapper_->start_transaction();
		}

		bool dboperator::stop_transaction()
		{
			return dbwrapper_->stop_transaction();
		}

		bool dboperator::commit()
		{
			return dbwrapper_->commit();
		}

		bool dboperator::rollback()
		{
			return dbwrapper_->rollback();
		}

	}
}

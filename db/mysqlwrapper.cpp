#include "mysqlwrapper.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#pragma comment(lib, "libmysql.lib")
#endif

namespace frog
{
	namespace db
	{

		mysqlwrapper::mysqlwrapper()
			:db_(NULL),resultset_(NULL),affected_rowcount_(0)
		{ }

		mysqlwrapper::~mysqlwrapper()
		{
            disconnect();
            clear();
        }

        bool mysqlwrapper::connect(const std::string& host,
                                   const std::string& user,
                                   const std::string& password,
                                   const std::string& dbname,
                                   unsigned int port,
                                   const std::string& unix_socket,
                                   const std::string& character)
		{
            db_ = mysql_init(db_);
			if(NULL == db_)
			{ return false; }
            
			my_bool autoreconnect = 1;
			mysql_options(db_, MYSQL_OPT_RECONNECT, &autoreconnect);
            
			if(NULL == mysql_real_connect(db_,
                                          host.c_str(),
                                          user.c_str(),
                                          password.c_str(),
                                          dbname.c_str(),
                                          port,
                                          unix_socket.c_str(),
                                          CLIENT_MULTI_RESULTS|CLIENT_MULTI_STATEMENTS))
			{ return false; }
            
			mysql_set_character_set(db_, character.c_str());
            
            if(mysql_ping(db_) != 0)
            {
                //std::cout << mysql_error(db_) << std::endl;
                return false;
            }
            
            mysql_autocommit(db_,true);
            
			return true;
		}
        
		bool mysqlwrapper::disconnect()
		{
			if(db_)
			{
				mysql_close(db_);
				db_ = NULL;
			}
			return true;
		}
        
		dataset& mysqlwrapper::get_dataset()
		{ return dataset_; }

		bool mysqlwrapper::rollback()
		{
			const char* statement = "ROLLBACK;";
			if (mysql_real_query(db_,statement,(unsigned int)strlen(statement)))
			{ return false; }
			return true;
		}

		bool mysqlwrapper::commit()
		{
			const char* statement = "COMMIT;";
			if (mysql_real_query(db_,statement,(unsigned int)strlen(statement)))
			{ return false; }
			return true;
		}

		bool mysqlwrapper::start_transaction()
		{
			const char* statement = "SET AUTOCOMMIT =0";
			if(mysql_real_query(db_,statement,(unsigned int)strlen(statement)))
			{ return false; }
			
			//const char* beginstatement = "Begin ;";
			//if(mysql_real_query(db_,beginstatement,(unsigned int)strlen(beginstatement)))
			//{ return false; }

			return true;
		}

		bool mysqlwrapper::stop_transaction()
		{
			const char* statement = "SET AUTOCOMMIT =1";
			if(mysql_real_query(db_,statement,(unsigned int)strlen(statement)))
			{ return false; }

			return true;
		}

		bool mysqlwrapper::execute(const std::string &sql)
		{
			clear();

			if(0 == mysql_query(db_,sql.c_str()))
			{
				do {
					affected_rowcount_ +=(unsigned long)mysql_affected_rows(db_);
				} while(!mysql_next_result(db_));
			}
			else
			{
				//std::cout << mysql_error(db_) << std::endl;
				return false;
			}

			return true;
		}

		bool mysqlwrapper::query(const std::string& sql)
		{
			clear();

			if(mysql_query(db_,sql.c_str()) != 0)
			{
				//std::cout << mysql_error(db_) << std::endl;
				return false;
			}

			do {
				resultset_ = mysql_store_result(db_);
				if(resultset_ == NULL) { continue; }

				affected_rowcount_ += (unsigned long)mysql_affected_rows(db_);

				for(MYSQL_FIELD* fieldptr = mysql_fetch_field(resultset_); fieldptr != NULL; fieldptr = mysql_fetch_field(resultset_))
				{
					column_t col;
					col.length = fieldptr->length;
					col.name = fieldptr->name;
					dataset_.push_column(col);
				}

				for(MYSQL_ROW currrow = mysql_fetch_row(resultset_); currrow != NULL; currrow = mysql_fetch_row(resultset_) )
				{
					const unsigned int colcount = mysql_num_fields(resultset_);
					row_t row(colcount);
					for(unsigned int i = 0; i < colcount; ++i)
					{ row[i] = (currrow[i] ? currrow[i] : ""); }
					dataset_.push_row(row);
				}

				mysql_free_result(resultset_); 
				resultset_ = NULL;
			} while(!mysql_next_result(db_));

			return true;
		}

		unsigned long mysqlwrapper::affected_rowcount()
		{ return affected_rowcount_; }

		void mysqlwrapper::clear()
		{
			dataset_.clear_all();
			affected_rowcount_ = 0;
		}

	}
}



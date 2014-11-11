//
//  postgresqlwrapper.cpp
//  console
//
//  Created by huanghaifeng on 14-10-24.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "postgresqlwrapper.h"
#include "scoped_cleaner.h"
#include <boost/bind.hpp>

using namespace frog::utils;

namespace frog
{
    namespace db
    {
        
        postgresqlwrapper::postgresqlwrapper()
            :db_(NULL),resultset_(NULL),affected_rowcount_(0)
        {
            
        }
        
        postgresqlwrapper::~postgresqlwrapper()
        {
            disconnect();
            clear();
        }
        
        bool postgresqlwrapper::connect(const std::string& host,
                                        const std::string& user,
                                        const std::string& password,
                                        const std::string& dbname,
                                        unsigned int port,
                                        const std::string& unix_socket,
                                        const std::string& character)
        {
            char cfg[512] = {0};
            sprintf(cfg, "host=%s port=%d dbname=%s user=%s password=%s",
                    host.c_str(), port, dbname.c_str(), user.c_str(), password.c_str());
            
            db_ = PQconnectdb(cfg);
            if(PQstatus(db_) == CONNECTION_BAD)
            {
                //fprintf(stderr,"%s\n",PQerrorMessage(db_));
                return false;
            }
            // charset ?
            return true;
        }
        
        bool postgresqlwrapper::disconnect()
        {
            if (db_)
            {
                PQfinish(db_);
                db_ = NULL;
            }
            return false;
        }
        
        dataset& postgresqlwrapper::get_dataset()
        {
            return dataset_;
        }
        
        bool postgresqlwrapper::rollback()
        {
            const char* statement = "ROLLBACK;";
            resultset_ = PQexec(db_, statement);
            utils::scoped_cleaner gc(boost::bind(&PQclear, resultset_));//required?
            if (PQresultStatus(resultset_) == PGRES_FATAL_ERROR)
            {
                return false;
            }
            return true;
        }
        
        bool postgresqlwrapper::commit()
        {
            const char* statement = "COMMIT;";
            resultset_ = PQexec(db_, statement);
            utils::scoped_cleaner gc(boost::bind(&PQclear, resultset_));//required?
            if (PQresultStatus(resultset_) == PGRES_FATAL_ERROR)
            {
                return false;
            }
            return true;
        }
        
        bool postgresqlwrapper::start_transaction()
        {
            const char* statement = "START TRANSACTION;";
            resultset_ = PQexec(db_, statement);
            utils::scoped_cleaner gc(boost::bind(&PQclear, resultset_));//required?
            if (PQresultStatus(resultset_) == PGRES_FATAL_ERROR)
            {
                return false;
            }
            return true;
        }
        
        bool postgresqlwrapper::stop_transaction()
        {
            // do nothing ?
            return true;
        }
        
        bool postgresqlwrapper::execute(const std::string& sql)
        {
            clear();
            
            resultset_ = PQexec(db_, sql.c_str());
            utils::scoped_cleaner gc(boost::bind(&PQclear, resultset_));//required?
            
            if (PQresultStatus(resultset_) == PGRES_FATAL_ERROR)
            {
                //fprintf(stderr, "%s", PQerrorMessage(db_));
                return false;
            }
            
            if (PQresultStatus(resultset_) == PGRES_COMMAND_OK)
            {
                affected_rowcount_ = string_to_number<unsigned long>(PQcmdTuples(resultset_));
                return true;
            }
            return false;
        }
        
        bool postgresqlwrapper::query(const std::string& sql)
        {
            clear();
            
            resultset_ = PQexec(db_, sql.c_str());
            utils::scoped_cleaner gc(boost::bind(&PQclear, resultset_));
            
            if (PQresultStatus(resultset_) == PGRES_FATAL_ERROR)
            {
                //fprintf(stderr, "%s", PQerrorMessage(db_));
                return false;
            }
            
            if (PQresultStatus(resultset_) == PGRES_TUPLES_OK)
            {
                affected_rowcount_ = string_to_number<unsigned long>(PQcmdTuples(resultset_));
                
                int colcount = PQnfields(resultset_);
                int rowcount = PQntuples(resultset_);
                
                for (int ci=0; ci<colcount; ++ci) {
                    column_t col;
					col.length = PQfsize(resultset_,ci);
					col.name = PQfname(resultset_,ci);
					dataset_.push_column(col);
                }
                
                for (int i=0; i<rowcount; i++)
                {
                    row_t row(colcount);
                    for (int j=0; j<colcount; j++)
                    {
                        row[j] = PQgetvalue(resultset_, i, j);
                    }
                    dataset_.push_row(row);
                }
                return true;
            }
            return false;
        }
        
        unsigned long postgresqlwrapper::affected_rowcount()
        {
            return affected_rowcount_;
        }
        
        void postgresqlwrapper::clear()
        {
            dataset_.clear_all();
			affected_rowcount_ = 0;
        }
        
    }
}


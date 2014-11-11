#ifndef DBFACTORY_HEADER
#define DBFACTORY_HEADER

#include "mysqlwrapper.h"
#include "postgresqlwrapper.h"
#include "oraclewrapper.h"

namespace frog
{
	namespace db
	{

        enum dbtype
        {
            e_mysql,
            e_postgre,
            e_oracle,
        };
        
		namespace dbfactory
		{
			inline idbwrapper* create_dbwrapper(dbtype dt)
			{
                switch (dt) {
                    case e_mysql:
                        return new mysqlwrapper();
                        
                    case e_postgre:
                        return new postgresqlwrapper();
                        
                    case e_oracle:
                        return new oraclewrapper();
                        
                    default:
                        break;
                }
				return NULL;
			}

			inline void dispose_dbwrapper(idbwrapper** wrapper)
			{
				delete *wrapper;
				*wrapper = NULL;
			}

		}

	}
}

#endif

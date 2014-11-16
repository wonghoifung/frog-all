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
            e_mysql = 0,
            e_postgre = 1,
            e_oracle = 2,
            e_unknown = 3,
        };
        
        inline dbtype dbtype_cast(int type)
        {
            switch (type) {
                case e_mysql:
                    return e_mysql;
                case e_postgre:
                    return e_postgre;
                case e_oracle:
                    return e_oracle;
                default:
                    break;
            }
            return e_unknown;
        }
        
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

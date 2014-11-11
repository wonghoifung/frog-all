//
//  oraclewrapper.h
//  console
//
//  Created by huanghaifeng on 14-10-25.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef oraclewrapper_header
#define oraclewrapper_header

#include "idbwrapper.h"

namespace frog
{
    namespace db
    {
        
        class oraclewrapper : public boost::noncopyable, public idbwrapper
        {
        public:
            oraclewrapper();
            virtual ~oraclewrapper();
            
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
            dataset dataset_;
			unsigned long affected_rowcount_;
        };
        
    }
}

#endif


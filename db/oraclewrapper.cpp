//
//  oraclewrapper.cpp
//  console
//
//  Created by huanghaifeng on 14-10-25.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#include "oraclewrapper.h"

namespace frog
{
    namespace db
    {
        
        oraclewrapper::oraclewrapper():affected_rowcount_(0)
        {
            
        }
        
        oraclewrapper::~oraclewrapper()
        {
            disconnect();
            clear();
        }
        
        bool oraclewrapper::connect(const std::string& host,
                                    const std::string& user,
                                    const std::string& password,
                                    const std::string& dbname,
                                    unsigned int port,
                                    const std::string& unix_socket,
                                    const std::string& character)
        {
            // TODO
            return false;
        }
        
        bool oraclewrapper::disconnect()
        {
            // TODO
            return false;
        }
        
        dataset& oraclewrapper::get_dataset()
        {
            return dataset_;
        }
        
        bool oraclewrapper::rollback()
        {
            // TODO
            return false;
        }
        
        bool oraclewrapper::commit()
        {
            // TODO
            return false;
        }
        
        bool oraclewrapper::start_transaction()
        {
            // TODO
            return false;
        }
        
        bool oraclewrapper::stop_transaction()
        {
            // TODO
            return false;
        }
        
        bool oraclewrapper::execute(const std::string& sql)
        {
            // TODO
            return false;
        }
        
        bool oraclewrapper::query(const std::string& sql)
        {
            // TODO
            return false;
        }
        
        unsigned long oraclewrapper::affected_rowcount()
        {
            return affected_rowcount_;
        }
        
        void oraclewrapper::clear()
        {
            dataset_.clear_all();
			affected_rowcount_ = 0;
        }
        
    }
}



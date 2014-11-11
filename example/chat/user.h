//
//  user.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef peer_header
#define peer_header

namespace frog
{
    namespace chat
    {
    
        class user
        {
        public:
            user():id_(0){}
            ~user(){}
            
            void set_id(int i) {id_=i;}
            int get_id() {return id_;}
            
        private:
            int id_;
        };
        
    }
}

#endif

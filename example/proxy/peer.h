//
//  peer.h
//  console
//
//  Created by huanghaifeng on 14-11-8.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

#ifndef peer_header
#define peer_header

namespace frog
{
    namespace proxy
    {
        
        enum peer_type
        {
            peer_null = 0,
            peer_user = 1,
            peer_chat = 2,
            peer_mail = 3,
        };
        
        class peer
        {
        public:
            peer():peertype_(peer_null),id_(0){}
            ~peer(){}
            
            void set_peertype(peer_type pt) {peertype_=pt;}
            int get_peertype() {return peertype_;}
            
            void set_id(int i) {id_=i;}
            int get_id() {return id_;}
            
        private:
            int peertype_;
            int id_;
        };
        
    }
}

#endif

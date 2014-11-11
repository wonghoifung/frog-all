//
//  srsw_queue.hpp
//  console
//
//  Created by huanghaifeng on 14-10-23.
//  Copyright (c) 2014年 wonghoifung. All rights reserved.
//

/* single read single write lockfree queue */

#ifndef srsw_queue_header
#define srsw_queue_header

#include <boost/noncopyable.hpp>

namespace frog
{
    namespace utils
    {
        
        template <typename T>
        struct node
        {
            T data;
            node* next;
        };
        
        template <typename T>
        class srsw_queue : public boost::noncopyable
        {
        public:
            srsw_queue():head_(NULL),tail_(NULL) {}
            
            ~srsw_queue() {}
            
            bool init()
            {
                head_ = tail_ = new node<T>;
                if (tail_)
                {
                    tail_->next = NULL;
                    return true;
                }
                return false;
            }
            
            bool fini()
            {
                while (head_)
                {
                    tail_ = head_->next;
                    delete head_;
                    head_ = tail_;
                }
                return true;
            }
            
            bool push(T data)
            {
                node<T>* tn(new node<T>);
                if (tn)
                {
                    tn->next = NULL;
                    tail_->next = tn;  
                    tail_->data = data;
                    tail_ = tn;
                    return true;
                }
                return false;
            }
            
            bool pop(T& data)
            {
                if (head_ == tail_)
                { return false; }
                node<T>* tn(head_);
                data = tn->data;
                head_ = tn->next;
                delete tn;
                return true;
            }
            
        private:
            node<T>* volatile head_;
            node<T>* volatile tail_;
        };
        
    }
}

#endif 


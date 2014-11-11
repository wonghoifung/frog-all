//
//  srsw_circular_queue.hpp
//  console
//
//  Created by huanghaifeng on 14-10-24.
//  Copyright (c) 2014Äê wonghoifung. All rights reserved.
//

/* single read single write lockfree circular queue */

#ifndef SRSW_CIRCULAR_QUEUE_HEADER
#define SRSW_CIRCULAR_QUEUE_HEADER

#include <boost/noncopyable.hpp>

namespace frog
{
	namespace utils
	{

		template <typename T, unsigned SIZE>
		class srsw_circular_queue : public boost::noncopyable
		{
		public:
			srsw_circular_queue():head_(0),tail_(0)
			{ memset(data_,0,sizeof(data_)); }

			bool push(const T& data)
			{
				unsigned next((tail_ + 1) % (SIZE + 1));
				if (next == head_)
				{ return false; }
				data_[tail_] = data;
				tail_ = next;
				return true;
			}

			bool pop(T& data)
			{
				if (head_ == tail_)
				{ return false; }
				data = data_[head_];
				head_ = (head_ + 1) % (SIZE + 1);
				return true;
			}

		private:
			T data_[SIZE + 1];
			unsigned head_;
			unsigned tail_;
		};

	}
}

#endif

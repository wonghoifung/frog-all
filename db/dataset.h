#ifndef DATASET_HEADER
#define DATASET_HEADER

#include <string>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <boost/noncopyable.hpp>
#include "string_utils.h"

namespace frog
{
	namespace db
	{

		struct column_t
		{
			std::string name;
			unsigned long length;
			column_t():length(0) {}
		};
		
		typedef std::vector<std::string> row_t;
        
		class dataset : public boost::noncopyable
		{
		public:
			dataset();
			~dataset();
			void clear_all();
			unsigned long row_count();
			unsigned long column_count();
			const int32_t get_field_int32(unsigned long rowidx, const unsigned int columnidx);
			const int32_t get_field_int32(unsigned long rowidx, const std::string& fieldname);
			const int64_t get_field_int64(unsigned long rowidx, const unsigned int columnidx);
			const int64_t get_field_int64(unsigned long rowidx, const std::string& fieldname);
			const double get_field_double(unsigned long rowidx, const unsigned int columnidx);
			const double get_field_double(unsigned long rowidx, const std::string& fieldname);
			const std::string& get_field_string(unsigned long rowidx, unsigned int columnidx);
			const std::string& get_field_string(unsigned long rowidx, const std::string& fieldname);
			void push_column(const column_t& col);
			void push_row(const row_t& row);

		private:
			const unsigned int get_field_index_by_name(const std::string &fieldname);
			const std::string& get_field(unsigned long rowidx, unsigned int columnidx);
			const std::string& get_field(unsigned long rowidx, const std::string &fieldname);

		private:
			std::vector<column_t> columns_;
			std::vector<row_t> datas_;
		};
        
	}
}

#endif

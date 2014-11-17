#include "dataset.h"

using namespace frog::utils;

namespace
{
	const std::string null_string = std::string();
	const unsigned int null_columnidx = 888888;
}

namespace frog
{
	namespace db
	{

		dataset::dataset()
		{
		}

		dataset::~dataset()
		{
		}

		void dataset::clear_all()
		{
			std::vector<column_t>().swap(columns_);
			std::vector<row_t>().swap(datas_);
		}

		unsigned long dataset::row_count()
		{ return datas_.size(); }

		unsigned long dataset::column_count()
		{ return columns_.size(); }

		const int32_t dataset::get_field_int32(unsigned long rowidx, const unsigned int columnidx)
		{ return string_to_number<int32_t>(get_field(rowidx, columnidx)); }

		const int32_t dataset::get_field_int32(unsigned long rowidx, const std::string& fieldname)
		{ return string_to_number<int32_t>(get_field(rowidx, fieldname)); }

		const int64_t dataset::get_field_int64(unsigned long rowidx, const unsigned int columnidx)
		{ return string_to_number<int64_t>(get_field(rowidx, columnidx)); }

		const int64_t dataset::get_field_int64(unsigned long rowidx, const std::string& fieldname)
		{ return string_to_number<int64_t>(get_field(rowidx, fieldname)); }

		const double dataset::get_field_double(unsigned long rowidx, const unsigned int columnidx)
		{ return string_to_number<double>(get_field(rowidx, columnidx)); }

		const double dataset::get_field_double(unsigned long rowidx, const std::string& fieldname)
		{ return string_to_number<double>(get_field(rowidx, fieldname)); }

		const std::string& dataset::get_field_string(unsigned long rowidx, unsigned int columnidx)
		{ return get_field(rowidx, columnidx); }

		const std::string& dataset::get_field_string(unsigned long rowidx, const std::string& fieldname)
		{ return get_field(rowidx, fieldname); }

		void dataset::push_column(const column_t& col) 
		{ columns_.push_back(col); }

		void dataset::push_row(const row_t& row) 
		{ datas_.push_back(row); }

		const unsigned int dataset::get_field_index_by_name(const std::string &fieldname)
		{
			for(unsigned int i = 0; i < columns_.size(); ++i)
			{
				if(columns_[i].name == fieldname)
				{ return i; }
			}
			return null_columnidx;
		}

		const std::string& dataset::get_field(unsigned long rowidx, unsigned int columnidx)
		{
			if(columnidx >= column_count() || rowidx >= row_count())
			{ return null_string; }
			return datas_[rowidx][columnidx];
		}

		const std::string& dataset::get_field(unsigned long rowidx, const std::string &fieldname)
		{
			unsigned int columnidx = get_field_index_by_name(fieldname);
			if(columnidx >= null_columnidx) { return null_string; }
			return get_field(rowidx, columnidx);
		}

	}
}

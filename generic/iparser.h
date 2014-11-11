#ifndef IPARSER_HEADER
#define IPARSER_HEADER

namespace frog
{
    namespace generic
	{

		class iparser
		{
		public:
			virtual ~iparser() {}
			virtual int parse(const char* data, const size_t length) = 0;
		};

	}
}

#endif

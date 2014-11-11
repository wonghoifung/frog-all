#ifndef TIME_UTILS_HEADER
#define TIME_UTILS_HEADER

#include <sstream>
#include <time.h>
#include <stdlib.h>

/* a naive version */

namespace frog
{
namespace utils
{

inline int get_timestamp(int year,int month,int day,int hour,int minute,int second);

inline int get_timestamp(const char* s)//input format: "YYYY-MM-DD hh:mm:ss"
{
	if (NULL==s)
	{
		return 0;
	}
	char dumpchar(0);
	int year(0),month(0),day(0),hour(0),minute(0),second(0);
	std::istringstream iss(s);
	iss >> year >> dumpchar >> month >> dumpchar >> day  
		>> hour >> dumpchar >> minute >> dumpchar >> second;
	return get_timestamp(year,month,day,hour,minute,second);
}

inline int get_timestamp(int year,int month,int day,int hour,int minute,int second)
{
	tm tmobj;
	tmobj.tm_year = year - 1900;
	tmobj.tm_mon = month - 1;
	tmobj.tm_mday = day;
	tmobj.tm_hour = hour;
	tmobj.tm_min = minute;
	tmobj.tm_sec = second;
	tmobj.tm_isdst = -1;
	return mktime(&tmobj);
}

inline void get_time_details(time_t* t_, int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	//char szTime[20]={0};
	struct tm* t = localtime(t_);
	year = 1900 + t->tm_year;
	month = t->tm_mon + 1;
	day = t->tm_mday;
	hour = t->tm_hour;
	minute = t->tm_min;
	second = t->tm_sec;
}

inline void get_time_details(int* t, int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	time_t tt = (time_t)*t;
	get_time_details(&tt,year,month,day,hour,minute,second);
}

inline std::string get_string_time(time_t* time_)
{
	char szTime[20]={0};
	struct tm* t = localtime(time_);
	sprintf(szTime, "%d-%02d-%02d %02d:%02d:%02d",
		1900 + t->tm_year, t->tm_mon+1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec);
	return szTime;
}

inline std::string get_string_time(time_t time_)
{
    return get_string_time(&time_);
}
    
inline std::string get_string_time(int* time_)
{
	time_t t = (time_t)*time_;
	return get_string_time(&t);
}

inline std::string get_string_time(int time_)
{
	return get_string_time(&time_);
}

inline bool is_today(int timestamp)
{
	time_t current(time(0));
	time_t ts = (time_t)timestamp;
	int today_year(0),today_month(0),today_day(0),ts_year(0),ts_month(0),ts_day(0),dump(0);
	get_time_details(&current,today_year,today_month,today_day,dump,dump,dump);
	get_time_details(&ts,ts_year,ts_month,ts_day,dump,dump,dump);
	return (today_year == ts_year && today_month == ts_month && today_day == ts_day);
}

inline int get_left_seconds_today()
{
	time_t current(time(0));
	int year(0),month(0),day(0),hour(0),minute(0),second(0);
	get_time_details(&current, year, month, day, hour, minute, second);
	int day_end = get_timestamp(year,month,day+1,0,0,0);
	return day_end - current;
}

}
}

#endif


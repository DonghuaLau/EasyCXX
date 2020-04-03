#ifndef _EC_REACTOR_ERROR_H_
#define _EC_REACTOR_ERROR_H_

#include <map>
using namespace std;

namespace ec
{

class EventError
{
public:
	enum EN_ERRNO
	{
		E_OK			= 0,
		E_FAILED		= 1,
		E_ALLOCATE		= 2,
		E_INVALID_PARAM	= 3,
		E_ADD_EVENT		= 4,
		E_INVALID_VAL	= 5,
	};

	static string & get_errmsg(int error)
	{
		EventError::errmsg_init();
		map<int, string>::iterator it = EventError::_map_errmsg.find(error);
		if(it != EventError::_map_errmsg.end())
		{
			return it->second;
		}
		static string s = "internal error";
		return s;
	}

private:
	static map<int, string> _map_errmsg;

	static void errmsg_init()
	{
		if(EventError::_map_errmsg.size() > 0) return;
		EventError::_map_errmsg[E_OK] = "OK";
		EventError::_map_errmsg[E_FAILED] = "failed";
	}
};

map<int, string> EventError::_map_errmsg;

}; // namespace ec

#endif



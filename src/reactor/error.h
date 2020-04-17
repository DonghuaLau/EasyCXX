#ifndef _EC_REACTOR_ERROR_H_
#define _EC_REACTOR_ERROR_H_

#include <map>
#include <string>
using namespace std;

namespace ec
{

class EventError
{
public:
	enum EN_ERRNO
	{
		E_OK				= 0,
		E_FAILED			= -1001,
		E_ALLOCATE			= -1002,
		E_INVALID_PARAM		= -1003,
		E_ADD_EVENT			= -1004,
		E_INVALID_VAL		= -1005,
		E_SCOK_CREATE		= -1006,
		E_SCOK_BIND			= -1007,
		E_SCOK_LISTEN		= -1008,
		E_SCOK_ACCEPT		= -1009,
		E_SOCK_READ			= -1010,
		E_SOCK_WRITE		= -1011,
		E_EVENT_UNSPPORTED	= -1012,
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

		EventError::_map_errmsg.insert(pair<int, string>(E_OK, "OK"));;
		EventError::_map_errmsg.insert(pair<int, string>(E_FAILED, "failed"));;
	}
};

map<int, string> EventError::_map_errmsg;

}; // namespace ec

#endif



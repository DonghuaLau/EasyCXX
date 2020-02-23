#include <iostream>
#include <sstream>

/*
 *	规则：
 *		|type1|data1|type2|data2|...
 *		type: 1|bool, 2|char, 3|short, 4|int, 5|long long, 6|double
 *			  21|vector, 22|map, 23|struct
 */

enum EN_SERI_DATA_TYPE
{
	SERI_BOOL		= 1,
	SERI_CHAR		= 2,
	SERI_SHORT		= 3,
	SERI_INT		= 4,
	SERI_LONGLONG	= 5,
	SERI_DOUBLE		= 6,

	SERI_VECTOR		= 21,
	SERI_MAP		= 22,
	SERI_STRUCT		= 23,
};

class CSerialize
{
public:
	CSerialize():_pos(0){}

	string getBuf()
	{
		return _buf;
	}

	void wirte(const bool n)
	{
		
	}

	void wirte(const char n)
	{
		
	}

	void wirte(const short n)
	{
		
	}

	void wirte(const int n)
	{
		
	}


private:
	string _buf;
	int _pos;
};

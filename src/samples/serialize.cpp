#include <iostream>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

/*
 *	规则：
 *		|type1|len1|[subtype1|]data1|type2|len2[subtype2|]data2|...
 *		type: 1|bool, 2|char, 3|short, 4|int, 5|long, 6|double, 7|long double (无符号类型由代码处理)
 *			  10|string, 11|vector, 12|map, 13|struct
 *		
 *		1.基本类型时，type和len共占用一个字节，用T表示。
 *			1.1 T:0 - 总是0
 *			1.2 T:1-3 - type(1-7)
 *			1.3 T:4-7 - len(n+1个字节, long double是16字节，这样刚好可以表示完，后面为了和这里保持一致)
 *		2.其它类型，type占一个字节, len为变长，用T表示type所在字节。
 *			2.1 T:0 - 总是1
 *			2.2 T:1-3 - len的长度(最大4个字节, 取值1-4, n+1个字节, vector和map时表示元素数量)
 *			2.3 T:4-7 - type(10-15)
 *		3.subtype字段，type为11-12时需要。
 *			3.1 type=11: subtype为一个字节，取值同type(1-7,10,13), 不支持嵌套vector和map。
 *				3.1.1 data数据格式：sublen1|subdata1|sublen2|subdata2|...
 *				3.1.2 sublen: 最高两位表示占用的字节数，含当前字节，即最长支持0x3fffffff字节
 *				3.1.3 subtype=13时，subdata递归些规则。
 *			3.2 type=12: subtype为一个字节，高4位和低4位分别表示key和value的类型，同type(1-7,10,13), 不支持嵌套。
 *				3.1.1 data数据格式：keylen1|keydata1|valuelen1|valuedata1|keylen2|keydata2|valuelen2|valuedata2|...
 *				3.1.2 keylen,valuelen的规则同subtype=11的sublen.
 *				3.1.3 keydata,valuedata的规则同subtype=11的subdata.
 */

enum EN_SERI_DATA_TYPE
{
	SERI_BOOL		= 1,
	SERI_CHAR		= 2,
	SERI_SHORT		= 3,
	SERI_INT		= 4,
	SERI_LONG		= 5,
	SERI_DOUBLE		= 6,
	SERI_LONG_DOUBLE= 7,

	SERI_VECTOR		= 21,
	SERI_MAP		= 22,
	SERI_STRUCT		= 23,
};

const int SEGMENT_SIZE = 512;

#define return_failed(expr, err) if(expr){ return err; }

class CSingleEncode
{
public:
	static int encode(const bool n, char *buf, const int len)
	{
		return_failed(len < 1, -1);
		buf[0] = 0b00010000;
		buf[1] = n;
		return 2;
	}

	static int encode(const char n, char *buf, const int len)
	{
		return_failed(len < 1, -1);
		buf[0] = 0b00100000;
		buf[1] = n;
		return 2;
	}

	static int encode(const short n, char *buf, const int len)
	{
		if((n & 0xff00) == 0)
		{
			return_failed(len < 2, -1);
			buf[0] = 0b00110000;			
			buf[1] = (char)n;
			return 2;
		}
		else
		{
			return_failed(len < 3, -1);
			buf[0] = 0b00110001;			
			buf[1] = n & 0xff00;
			buf[2] = n & 0x00ff;
			return 3;
		}
		return 0;
	}

	static int encode(const int n, char *buf, const int len)
	{
		return_failed(sizeof(n) > 4, -1);

		if((n & 0xffffff00) == 0)
		{
			return_failed(len < 2, -1);
			buf[0] = 0b01000000;			
			buf[1] = n & 0x00ff;
			return 2;
		}
		else if((n & 0xffff0000) == 0)
		{
			return_failed(len < 3, -1);
			buf[0] = 0b01000001;
			buf[1] = n >> 1;
			buf[2] = n;
			return 3;
		}
		else if((n & 0xff000000) == 0)
		{
			buf[0] = 0b01000010;
			buf[1] = n & 0xff0000;
			buf[2] = n & 0x00ff00;
			buf[3] = n & 0x0000ff;
			return 4;
		}
		else if((n & 0xff000000) == 0)
		{
			buf[0] = 0b01000011;
			buf[1] = n & 0xff000000;
			buf[2] = n & 0x00ff0000;
			buf[3] = n & 0x0000ff00;
			buf[4] = n & 0x000000ff;
			return 5;
		}
		return 0;
	}
};

class CByteBuffer
{
public:
	CByteBuffer():_buf(NULL),_size(0),_capacity(SEGMENT_SIZE)
	{
		_buf = (char*)malloc(_capacity);
	}

	~CByteBuffer()
	{
		if(_buf)
		{
			free(_buf);
			_buf = NULL;
		}
	}

	int getBuf(char *buf)
	{
		buf = _buf;
		return _size;
	}

	int write(const char *buf, const int len)
	{
		resize_if_necessary(len);
		memcpy(_buf + _size, buf, len);
		_size += len;
		return len;;
	}

private:
	void resize_if_necessary(const int len)
	{
		if(_size + len <= _capacity)
		{
			return;
		}
		int new_capacity = (int)(_capacity * 1.5);
		_buf = (char*)realloc((void*)_buf, new_capacity);
		_capacity = new_capacity;
	}

private:
	char *_buf;
	int _size;
	int _capacity;
	
};

class CSerialize
{
public:
	CSerialize() {}

	int getBuf(char *buf)
	{
		return _bbuf.getBuf(buf);
	}

	virtual int writeTo() = 0;

private:
	void writeBool(const bool n)
	{
		cout << "write bool" << endl;		
		char buf[2] = {0};
		buf[0] = n;
		_bbuf.write(buf, 2);
	}

	void writeChar(const char n)
	{
		cout << "write char" << endl;		
		char buf[2] = {0};
		buf[0] = n;
		_bbuf.write(buf, 2);
	}

	void writeShort(const short n)
	{
		cout << "write short" << endl;		
		char buf[3] = {0};
		int len = GET_COMPRESS_LEN(n);	
		_bbuf.write(buf, 3);
	}

	void writeInt(const int n)
	{
		cout << "write int" << endl;		
	}

	void writeLong(const long n)
	{
		
		cout << "write long" << endl;		
	}

	void writeDouble(const double n)
	{
		cout << "write double" << endl;		
	}

	void writeLongDouble(const long double n)
	{
		cout << "write long double" << endl;		
	}

	void writeString(const string &s)
	{
		cout << "write string" << endl;		
	}

protected:
	template<typename T>
	void write(const T & t)
	{
		//TODO: unsigned

		const type_info & tType = typeid(t);
		cout << "write custom, type: " << tType.name() << endl;		
		if(tType == typeid(int))
		{
			//cout << "write int" << endl;		
			writeInt(t);	
		}
		else if(tType == typeid(string))
		{
			cout << "write string" << endl;		
		}
		else if(tType == typeid(long))
		{
			cout << "write string" << endl;		
		}
		else if(tType == typeid(double))
		{
			cout << "write short" << endl;		
		}
		else if(tType == typeid(short))
		{
			cout << "write short" << endl;		
		}
		else if(tType == typeid(bool))
		{
			// 无法识别bool ？
			cout << "write bool" << endl;		
		}
		else if(tType == typeid(char))
		{
			cout << "write char" << endl;		
		}
		else if(tType == typeid(long double))
		{
			cout << "write short" << endl;		
		}
	}

	template<typename T>
	void write(const vector<T> & v)
	{
		cout << "write vector" << endl;		
		//cout << "type: " << typeid(T).name() << endl;		
		write(v[0]);
	}

	template<typename TK, typename TV>
	void write(const map<TK, TV> & m)
	{
		
	}

private:
	void writeBuf(int );

private:
	CByteBuffer _bbuf;
};

class CInfo : public CSerialize
{
public:
	CInfo(){}
	virtual ~CInfo(){}

	
	virtual int writeTo()
	{
		write(id_list);
		return 0;
	}
	
public:
	int id;
	string name;
	//vector<bool> id_list;
	vector<int> id_list;
	//vector<string> id_list;
};

class CSub : public CSerialize
{

public:
	int ret;
	string msg;
};

int main()
{
	/*string s;
	s.push_back(5);
	s.push_back(96);
	cout << s.size() << endl;*/

	//cout << typeid(bool).name() << endl;
	//cout << typeid(int).name() << endl;
	//cout << typeid(string).name() << endl;
	//cout << typeid(CInfo).name() << endl;

	CInfo info;
	//info.id_list.push_back(true);
	info.id_list.push_back(11);
	//info.id_list.push_back("11");
	info.writeTo();

	return 0;
}

#ifndef _EC_SMART_POINTER_H_
#define _EC_SMART_POINTER_H_

// Ref: http://www.cnblogs.com/QG-whz/p/4777312.html

#include <iostream>

template <typename T>
class ECSmartPointer;

template <typename T>
class ECPointerHelper
{
private:
	int _count;
	T *_p;

private:
	friend class ECSmartPointer<T>;

	ECPointerHelper(T *pointer) : _p(pointer), _count(1) { }

	~ECPointerHelper()
	{
		delete _p;
		_p = NULL;
		//std::cout << "delete object" << std::endl;
	}
};

template <typename T>
class ECSmartPointer
{
private:
	ECPointerHelper<T> *_helper;

public:

	ECSmartPointer(T *pointer) : _helper(new ECPointerHelper<T>(pointer)) { }

	ECSmartPointer(const ECSmartPointer<T> &smart_pointer) : _helper(smart_pointer._helper) // 构造函数不是会把_count设置为1吗？
	{
		++ _helper->_count;
	}

	ECSmartPointer & operator=(const ECSmartPointer<T> & smart_pointer)
	{
		++ smart_pointer->_count;
		if(-- _helper->_count == 0)
		{
			delete _helper;
			_helper = NULL;
			//std::cout << "delete helper object 1" << std::endl;
		}

		_helper = smart_pointer._helper;
		return *this;
	}
	
	T & operator *()
	{
		return *(_helper->_p);
	}

	T * operator ->()
	{
		return _helper->_p;
	}

	~ECSmartPointer()
	{
		if(--_helper->_count == 0)
		{
			delete _helper;
			_helper = NULL;
			//std::cout << "delete helper object 2" << std::endl;
		}
		else
		{
			//std::cout << "remain " << _helper->_count << " pointer(s) of object" << std::endl;
		}
	}
};

#endif


#ifndef _COMMON_TEST_H_
#define _COMMON_TEST_H_

#include <iostream>
#include <sstream>
#include <cstring>

class People {
public:
	std::string _name;
	int _age;

	std::string to_string()
	{
		std::stringstream ss;
		ss << "{ name: " << _name << ", age: " << _age << " }";
		return ss.str();
	}
}; 

#endif


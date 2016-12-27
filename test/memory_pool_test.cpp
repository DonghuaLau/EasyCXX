#include <iostream>
#include <sstream>
#include <cstring>
#include "common_test.h"
#include "memory_pool.h"

void test_1()
{
	std::cout << "[" << __func__ << "]" << std::endl;
	easycxx::EMemoryPool mp(10);

	int *p = NULL;
	p = mp.construct<int>();
	*p = 2;
	std::cout << "*p = " << *p << std::endl;
	mp.destory<int>(p);

	char *s = NULL;
	s = mp.construct<char>(3);
	s[0] = 'a';
	s[1] = 'b';
	s[2] = '\0';
	std::cout << "s = " << s << std::endl;
	mp.destory<char>(s);
}

void test_2()
{
	std::cout << "[" << __func__ << "]" << std::endl;
	easycxx::EMemoryPool mp(10);

	People *p = NULL;
	p = mp.construct<People>();
	p->_name = "Mike";
	p->_age = 28;
	std::cout << "people = " << p->to_string() << std::endl;

	mp.destory<People>(p);

}

int main()
{
	test_1();
	test_2();
	return 0;
}


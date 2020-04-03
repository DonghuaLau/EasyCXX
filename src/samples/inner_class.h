#ifndef _INNER_H_
#define _INNER_H_

#include <iostream>
using namespace std;

class AAA
{
public:
	class AA
	{
	public:
		void print();
	};

	void print();

private:
	AA _aa;
};

#endif

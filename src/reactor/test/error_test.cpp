#include <iostream>
#include "error.h"

int main()
{
	cout << ec::EventError::get_errmsg(ec::EventError::E_FAILED) << endl;
	cout << ec::EventError::get_errmsg(100) << endl;
	cout << ec::EventError::get_errmsg(-5) << endl;
}


#include <iostream>
#include "inner_class.h"
using namespace std;

void AAA::print()
{
	cout << "Hello, this is class AAA" << endl;
	_aa.print();
}

void AAA::AA::print()
{
	cout << "Hello, this is class AA" << endl;
}

int main()
{
	AAA a;
	a.print();
}

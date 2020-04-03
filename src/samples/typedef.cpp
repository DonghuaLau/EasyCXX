#include <iostream>
using namespace std;

class A
{
public:
	void print()
	{
		cout << "Hello, this is class A" << endl;
	}
};

typedef A B;

int main()
{
	A a;
	B b;
	a.print();
	b.print();
}

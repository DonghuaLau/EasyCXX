#include <iostream>
using namespace std;

class A
{
	int a;
	string b;
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

	shared_ptr<B> pb;
	pb = make_shared<B>();
	pb->print();
}

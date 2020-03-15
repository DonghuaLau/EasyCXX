#include <iostream>
using namespace std;

class B
{
public:
	virtual void print()
	{
		cout << "in B::print()" << endl;
	}
	virtual void funcB()
	{
		cout << "in B::funcB()" << endl;
	}
};

class C
{
public:
	virtual void print()
	{
		cout << "in C::print()" << endl;
	}
	virtual void funcC()
	{
		cout << "in C::funcC()" << endl;
	}
};

class D : public B, public C
{
public:
	virtual void print()
	{
		cout << "in D::print()" << endl;
		B::print();
		C::print();
	}
	virtual void funcB()
	{
		cout << "in D::funcB()" << endl;
	}
	virtual void funcC()
	{
		cout << "in D::funcC()" << endl;
		int *p = new int[10];
		p[300000] = 5;
	}
private:
	int n = 5;
};

int main()
{
	D *d;
	d = new D();
	d->print();
	d->funcB();
	d->funcC();

	return 0;
}

#include <iostream>
using namespace std;

class A
{
public:
	virtual void print()
	{
		cout << "in A::print()" << endl;
	}
};

class B : public virtual A
{
public:
	virtual void print()
	{
		cout << "in B::print()" << endl;
	}
};

class C : public virtual A
{
public:
	virtual void print()
	{
		cout << "in C::print()" << endl;
	}
};

class D : public B, public C
{
public:
	virtual void print()
	{
		cout << "in D::print()" << endl;
		A::print();
		B::print();
		C::print();
	}
};

int main()
{
	A *a;
	a = new D();
	a->print();

	return 0;
}

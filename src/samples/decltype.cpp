#include <iostream>
using namespace std;

class A
{
	A *_a = NULL;
public:
	virtual void print()
	{
		cout << "print in class A, typeid: " << typeid(*this).name() << endl;
		if(_a == NULL)
		{
			//_a = new std::remove_pointer<decltype(this)>::type();
			//_a = new std::remove_reference<decltype(*this)>::type();
			_a = create();
			_a->print1();
		}
	}

	virtual void print1()
	{
		cout << "print1 in class A, typeid: " << typeid(*this).name() << endl;
	}

	virtual A *create() = 0;
};


class AA : public A
{
public:
	virtual void print1()
	{
		cout << "print1 in class AA, typeid: " << typeid(*this).name() << endl;
	}

	virtual A *create()
	{
		return new AA();
	}
};

class B
{
private:
	//shared_ptr<A> _a;
	A *_a;

public:
	void create1()
	{
		//_a = make_shared<A>();
		//cout << "1 typeid: " << typeid(*_a.get()).name() << endl;

		_a = new AA();
		cout << "1 typeid: " << typeid(_a).name() << endl;
		_a->print();
	}

	void create2()
	{
		//_a = make_shared<AA>();
		//cout << "2 typeid: " << typeid(_a).name() << endl;

		_a = new AA();
		cout << "2 typeid: " << typeid(*_a).name() << endl;
		_a->print();

		B *pb;
		//pb = new decltype((*this))();
		pb = new std::remove_pointer<decltype(this)>::type();
		pb->print();
	}

	virtual void print()
	{
		cout << "print in class B, typeid: " << typeid(*this).name() << endl;
	}
	
};



int main()
{
	/*
	B b;
	b.create1();
	b.create2();

	shared_ptr<decltype(b)> b1 = make_shared<decltype(b)>();
	b1->create1();
	b1->create2();
	*/

	//B *pb = new B();
	//pb->create1();
	//pb->create2();

	//shared_ptr<decltype(pb)> pb1 = make_shared<decltype(pb)>();
	//pb1->create1();
	//pb1->create2();

	A *a = new AA();
	cout << "2 typeid: " << typeid(*a).name() << endl;
	a->print();
}


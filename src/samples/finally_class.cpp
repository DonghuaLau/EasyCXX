#include <iostream>
using namespace std;

//A不能直接使用，会导致编译错误
template<typename T>
class A
{
private:
	A(){}
	~A(){}

public:
	virtual void print()
	{
		cout << "in A::print()" << endl;
	}

	friend T; //只有A的友类才可以构造A
};

//B是不可派生类
class B : public virtual A<B>
{
public:
	virtual void print()
	{
		cout << "in B::print()" << endl;
		A::print();
	}
};


int main()
{
	B *b;
	b = new B();
	b->print();
	return 0;
}


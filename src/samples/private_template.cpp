#include <iostream>
using namespace std;

class A
{
public:
	template<typename T1>
	void print1(T1 &&t)
	{
		cout << "A print1: " << t << endl;
	}

	template<typename T2>
	void print2(T2 &&t)
	{
		cout << "A print2: " << t << endl;
	}

private:
	template<typename T3>
	void print3(T3 &&t)
	{
		cout << "A print3: " << t << endl;
	}
};

template<typename T2>
void print2(double &&t) = delete;

int main()
{
	A a;
	a.print1<int>(36);
	a.print2<double>(9.86);
	//a.print3<string>("hello");
}

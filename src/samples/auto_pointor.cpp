#include <iostream>
#include <memory>

using namespace std;

void test_unique_ptr()
{
	std::unique_ptr<int> pn(new int);
	*pn = 5;
	cout << *pn << endl;
	std::unique_ptr<int> pn1(pn);
	cout << *pn1 << endl;
	cout << *pn << endl;
}

int main()
{
	test_unique_ptr();
	return 0;
}


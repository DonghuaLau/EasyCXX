#include <iostream>
using namespace std;

enum EN_A
{
	A_1 = 1,
	A_2 = 2,
};

enum EN_B : uint8_t
{
	B_101 = 101,
	B_102 = 102,
	B_103 = 103,
};

int main()
{
	cout << "sizeof(EN_A): " << sizeof(EN_A) << endl;
	cout << "sizeof(EN_B): " << sizeof(EN_B) << endl;
}

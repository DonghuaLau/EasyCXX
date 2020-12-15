#include <iostream>
using namespace std;

struct st_a
{
	uint32_t  __n;
    union {
        struct {
            uint32_t ir_freecount;   /* count of free inodes */
        } f;
        struct {
            uint16_t  ir_holemask;/* hole mask for sparse chunks */
            //uint8_t   ir_count;   /* total inode count */
            uint8_t   ir_freecount;   /* count of free inodes */
        } sp;
    } ir_u;
} st_a_t;

struct st_b
{
	//uint32_t  __n;
	uint64_t  __n64;
    union {
        struct {
            uint16_t  ir_holemask;/* hole mask for sparse chunks */
            uint8_t   ir_count;   /* total inode count */
            uint8_t   ir_freecount;   /* count of free inodes */
        } sp;
        struct {
            uint64_t ir_freecount;   /* count of free inodes */
            uint64_t ir_freecount1;   /* count of free inodes */
        } f;
    } ir_u;
} ;

int main()
{
	st_a a;
	a.__n = 5;
	a.ir_u.f.ir_freecount = 8;

	cout << "size: " << sizeof(st_b) << endl;

	cout << a.ir_u.f.ir_freecount << endl;
	cout << (int)a.ir_u.sp.ir_freecount << endl;
	cout << a.ir_u.sp.ir_holemask << endl;
}

/* Test of cosh() function.
   $Id$
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "progmem.h"

union lofl_u {
    long lo;
    float fl;
};

volatile union lofl_u v = { .lo = 1 };

PROGMEM const struct {		/* Table of test cases.	*/
    union lofl_u x;		/* argument	*/
    union lofl_u z;		/* cosh(x)	*/
    unsigned int maxulp;	/* max possible error in ULP	*/
} t[] = {

    { { .fl= 0 },	{ .fl= 1 },		0 },
    { { .fl= 1 },	{ .fl= 1.5430806 },	1 },
    { { .fl= 2 },	{ .fl= 3.7621957 },	1 },
    { { .fl= 5 },	{ .fl= 74.209949 },	2 },
    { { .fl= 10 },	{ .fl= 11013.233 },	7 },
    { { .fl= 20 },	{ .fl= 2.4258260e+8 },	20 },
    { { .fl= 50 },	{ .fl= 2.5923527e+21 },	20 },
    { { .fl= 85 },	{ .fl= 4.1115063e+36 },	40 },
    { { .fl= 89.25 },	{ .fl= 2.8823884e+38 }, 50 },
};

void x_exit (int index)
{
#ifndef	__AVR__
    fprintf (stderr, "t[%d]:  %#lx\n", index - 1, v.lo);
#endif
    exit (index ? index : -1);
}

int main ()
{
    union lofl_u x, z;
    unsigned int maxulp;
    unsigned long v1, z1, r;
    int i;
    
    for (i = 0; i < (int) (sizeof(t) / sizeof(t[0])); i++) {
	x.lo = pgm_read_dword (& t[i].x);
	z.lo = pgm_read_dword (& t[i].z);
	maxulp = pgm_read_word (& t[i].maxulp);
	v.fl = cosh (x.fl);
	
	v1 = (v.lo < 0) ? (unsigned long)~(v.lo) : v.lo + 0x80000000;
	z1 = (z.lo < 0) ? (unsigned long)~(z.lo) : z.lo + 0x80000000;
	r = (v1 >= z1) ? v1 - z1 : z1 - v1;
	
	if (r > maxulp) x_exit (i+1);
    }
    return 0;
}

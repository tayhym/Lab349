/*
 * math.h
 * Matthew Tay Han Yang (mhtay)
 * Deeptaanshu Kumar (deeptaan)
 * Kevin Brennan (kbrennan)
 */

#ifndef MATH_H_
#define MATH_H_

/*function prototypes to ensure compiler checks that the function
parameters are correct. */

/* adds argOne to argTwo and returns result.*/
int add(int argOne, int argTwo);

/* subtracts argTwo from argOne and returns result. */
int sub(int argOne, int argTwo);

/* multiplies argOne by argTwo and returns result.*/
int mul(int argOne, int argTwo);

/* divides argOne by argTwo and returns result*/
int cdiv(int argOne, int argTwo);

/* Finds (modulus) remainder after argOne minus integers numbers of argTwo */
int mod(int argOne, int argTwo);

#endif /* MATH_H_ */

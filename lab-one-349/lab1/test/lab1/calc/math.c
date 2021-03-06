/*
 * math.c
 * Matthew Tay Han Yang (mhtay)
 * Deeptaanshu Kumar (deeptaan)
 * Kevin Brennan (kbrennan)
 */
#include "math.h"

/* adds argOne to argTwo and returns result.*/
int add(int argOne, int argTwo) {
	return (argOne + argTwo);
}

/* subtracts argTwo from argOne and returns result. */
int sub(int argOne, int argTwo) {
	return (argOne - argTwo);
}

/* multiplies argOne by argTwo and returns result.*/
int mul(int argOne, int argTwo) {
	return (argOne*argTwo);
}

/* divides argOne by argTwo and returns result*/
int cdiv(int argOne, int argTwo) {
	return (argOne/argTwo);
}

/* Finds (modulus) remainder after argOne minus integers numbers of argTwo */
int mod(int argOne, int argTwo) {
	return (argOne%argTwo);
}

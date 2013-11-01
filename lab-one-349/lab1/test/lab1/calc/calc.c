/*
 * calc.c
 * Matthew Tay Han Yang (mhtay)
 * Deeptaanshu Kumar (deeptaan)
 * Kevin Brennan (kbrennan)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

#define TRUE 1
int main(int argc, char **argv) {
	//prevent buffOverflow attacks
	int maxBufSize = 10;
	char str[maxBufSize];
	char *result;
	int len;

	int numToks = 0;
	int argOne, argTwo;
	char operation;
	int ans;
	int numToksCheck = 0;
	int numToksCheckTwo = 0;

	while (TRUE) {
		result = fgets(str, maxBufSize, stdin);
		//check for failure in reading from command-line
		if (result == NULL) {
			exit(0);
		}
		else {
			// replace newline with null-terminator
			len = strlen(str);
			if (len>=1) {
				str[--len] = '\0';
			}
		}

		// read string tokens
		numToks = sscanf(str,"%d %c %d",&argOne,&operation,&argTwo);
		if (numToks != 3) {
			printf("Invalid type or number of operations. Exiting...\n");
			exit(0);
		}

		// check for extra characters (not valid input)
		char dummy;
		int dummyTwo;
		numToksCheck = sscanf(str,"%d %c %d %c",&argOne,&operation,&argTwo, &dummy);
		numToksCheckTwo = sscanf(str,"%d %c %d %d",&argOne,&operation,&argTwo, &dummyTwo);
		if (numToksCheck!=3 || numToksCheckTwo != 3) {
			printf("Invalid type or number of operations. Exiting...\n");
			exit(0);
		}

		// branch based on operation
		if (operation == '+') {
			ans = add(argOne, argTwo);
			printf("%d\n",ans);
		}

		else if (operation == '-') {
			ans = sub(argOne, argTwo);
			printf("%d\n",ans);
		}

		else if (operation == '/') {
			ans = cdiv(argOne, argTwo);
			printf("%d\n",ans);
		}
		else if (operation == '*') {
			ans = mul(argOne, argTwo);
			printf("%d\n",ans);
		}

		else if (operation == '%') {
			ans = mod(argOne, argTwo);
			printf("%d\n",ans);
		}
		else {
			printf("Error, Operation %c not supported\n", operation);
			exit(0);
		}
	}
}

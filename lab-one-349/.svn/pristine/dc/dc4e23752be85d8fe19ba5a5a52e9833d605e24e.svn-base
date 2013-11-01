/* andrewIDs: kbrennan mhtay deeptaan */
#include <stdlib.h>
#include <stdio.h>

/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
	int i, j;
	int foundInner;
	int result = 0;

	for (i = 0; i < len; i++) {
		foundInner = 0;
		for (j = 0; j < len; j++) {
			if (i == j) {
				continue;
			}
			if (arr[i] == arr[j]) {
				foundInner = 1;
			}
		}
		if (foundInner != 1) {
			result = arr[i];
		}
	}

	return result;
}

#endif

/* 
 * optimizationOne: i and j made count-down loops.
 * reduces add, cmp, bge to subs, blt.
 * optimizationTwo: avoid redundant check (i==j) that is computationally
 * expensive (n^2) complexity. instead, let foundInner be compared to 1
 * (only found ownself).
 * optimizationThree: avoid redundant local variable result. Return
 * value immediately. This reduces space (less assembly code and
 * 1 less register used to store result) and time (less data movement
 * instructions like mov).
 * optimizationFour: Loop unroll two times within innerLoop to minimize
 * cost of branching (subs and branching instructions, as well as pipeline flush when
 * wrong branch prediction). Optimizes time at expense of space. Space does not
 * worsen too much, as loop unroll only twice. Did not unroll the outer loop,
 * since handling the base case of i==0 would require another hardcoding of
 * the j for loop, which doubles the code length.
 *
 */

#ifdef OPTIMIZE1
int oddball(int *arr, int len) {
	/* given in spec that len = 2*n -1, n integer
	 * still within r4-r9 number of local variables
	 * (no register spilling)
	 */
	int i,j;
	int foundInner;

	for (i = len-1; i >=0; i--) {
		foundInner = 0;
		for (j = len-1; j > 1; j-=2) {
			if (arr[i] == arr[j]) {
				foundInner++;
			}
			if (arr[i] == arr[j-1]) {
				foundInner++;
			}

		}
		// check first element
		if (arr[i] == arr[0]) {
			foundInner++;
		}
		if (foundInner == 1) {
			return arr[i];
		}
	}

	return 0;
}
#endif

#ifdef OPTIMIZE2

/* Further Optimization from vTwo (see below for documentation):
 * Uses bit-wise xor with loop unrolling
 */
int oddball(int *arr, int len) {
	int i;
	int result = 0;
	for ( i = len-1; i > 1; i-=2 ) {
		result = result ^ arr[ i ];
		result = result ^ arr[i-1];
	}
	// handle 1st element
	result = result^arr[0];
	return result;
}

/* oddBall vTwo
 *
 * OptimiztionOne: Uses a different algorithm to compute the number that occurs only
 * once. Achieves O(n) complexity, instead of O(n^2) that is the worst
 * case run-time complexity of the optimize1 version. Computes the
 * expected total and the actual total after running the program. The
 * difference between the 2 numbers is the oddball. Expected total is
 * given by twice the arithmetic sum from 1 to N. Sum = N*(firstTerm + * lastTerm)
 *
 * OptimizationTwo: uses numLen to store calculations (len+1)/2. uses 1 extra local
 * variable instead of repeating the calculation in expectedSum. This is due to there
 * being Sufficient local variables available. code size decreased slightly.
 *
 * OptimizationThree: loop unroll twice, useful to increase speed when N is large, at
 * expense of a slight increase in code size.
 *
 *
 */

/*int oddballvTwo(int *arr, int len) {
 int i;
 int numLen = (len+1)/2;
 // sum = 2*(n/2)*(a1 + aN)
 int expectedSum = numLen*(1+numLen);
 int actualSum = 0;

 for (i=len-1;i>1;i-=2) {
 actualSum+=arr[i];
 actualSum+=arr[i-1];
 }
 // handle first element
 actualSum+= arr[0];
 return (expectedSum - actualSum);
 }
 */
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */

int randGenerator(int *arr, int len) {
	int i, j, r, rcount;
	for (i = 0; i < len; i++) {
		do {
			rcount = 0;
			r = rand() % (len / 2 + 1) + 1;
			for (j = 0; j < i && rcount < 2; j++) {
				if (arr[j] == r) {
					rcount++;
				}
			}
		} while (rcount >= 2);
		arr[i] = r;
		printf("%d ", r);
	}
	printf("\nDone generating\n");
	return 0;
}


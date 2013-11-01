#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strTableTwo.c"

/* This is the function to focus on for optimization */
extern void strTable(char *s, char *d, int sl, int dl);
extern void strTableTwo(char *s, char *d, int sl, int dl);

int main() {
	/* Assume that dst[i] is always >= 23 (decimal) */
	/* string two: "SomeTest!String" */
	/* string three: "aTreeAGoatALAMBaBoat" */
	/* string four: "abc"*/
	/* string five: ""*/
//	char src[] = "theinitialstring\n";
	char src[] = "SomeTest!String\n";
//	char src[] = "aTreeAGoatALAMBaBoat\n";
//	char src[] = "abc\n";
//	char src[] = "";

	char dst[] = "PENGUINS";
	int i, srcLen, dstLen;

	srcLen = ((strlen(src) >> 2) << 2); // srcLen = srcLen%4
	dstLen = ((strlen(dst) >> 2) << 2); // dstLen = destLen%4
	srcLenTwo = ((strlen(src) >> 2) << 2); // srcLen = srcLen%4
	dstLenTwo = ((strlen(dst) >> 2) << 2); // dstLen = destLen%4

	int iTwo, srcLenTwo, dstLenTwo;
//	char srcTwo[] = "theinitialstring\n";
	char srcTwo[] = "SomeTest!String\n";
//	char srcTwo[] = "aTreeAGoatALAMBaBoat\n";
//	char srcTwo[] = "abc\n";
//	char srcTwo[] = "";

	char dstTwo[] = "PENGUINS";
	printf("PRE: src: %s (%d bytes), dst: %s (%d bytes)\n", src, srcLen, dst,
			dstLen); // output src and dest len

	for (i = 0; i < 1000; i++) {
		strTable(src, dst, srcLen, dstLen); // copy src to dest
	// keep track of src (r0) dst (r1) srcLen (r2) and dstLen (r3)
		strTableTwo(srcTwo, dstTwo, srcLenTwo, dstLenTwo);
		
	}
	printf("POST: src: %s (%d bytes), dst: %s (%d bytes)\n", src, srcLen,
				dst, dstLen);

	printf("POST-Two(own): src: %s (%d bytes), dst: %s (%d bytes)\n", srcTwo, srcLenTwo,
			dstTwo, dstLenTwo);

	return 1;
}






/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Matthew Tay mhtay@andrew.cmu.edu
 *          Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 *          Kevin Brennan kbrennan@andrew.cmu.edu
 * Date:    11:53 10/10/2013
 */

#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	write(STDOUT_FILENO,argv[5],9);
	return argc;
	
	int i; 
	int strSize, numBytesRead; 
	for (i=0;i<argc;i++) {
		char *strMsg = argv[i];
 		// compute size of string 
		strSize = 0;
		while (strMsg[strSize]!=0x0) {
			strSize++;
		}
	
		numBytesRead = read(STDIN_FILENO, strMsg, strSize);
			
		if (numBytesRead <= 0) {
			break;
		}
		else {
			// put on STDOUT 
			write(STDOUT_FILENO, strMsg, numBytesRead);
			 				
			/* assumes all characters read are alphabets */
			/* block size 100 characters */
			char buf_mini[strSize];
			for (i=0;i<strSize;i++) {
				buf_mini[i] = strMsg[i];
			}
	
			char outBuf[strSize]; //@ remove +1
	
			/* rotate characters based on their ascii values */
			/* stop 1 short to avoid copying EOF */
			for (i = 0; i<strSize-1;i++) { 
				if (buf_mini[i]<='M' && buf_mini[i]>='A') {
					outBuf[i] = (char) ((int) buf_mini[i] +13);
				}
				else if (buf_mini[i]>='N' && buf_mini[i]<='Z') {
					outBuf[i] = (char) ((int) buf_mini[i] -13);
				}
				else if (buf_mini[i]>= 'a' && buf_mini[i]<= 'm') {
					outBuf[i] = (char) ((int) buf_mini[i] + 13);
				}
				else if (buf_mini[i] >= 'n' && buf_mini[i] <= 'z') {
					outBuf[i] = (char) ((int) buf_mini[i] - 13);
				}
				else {
					/* not a proper alphabet input */
					exit(1);
				}  
 			}
	
			outBuf[strSize-1] = '\n';
			write(STDOUT_FILENO, outBuf, strSize);
		}
	
	}

	return 0; // not reached 
}


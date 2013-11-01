/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Group Member 1 <email address>
 *          Group Member 2 <email address>
 * Date:    The current time & date
 */

#include  <unistd.h>
#include <stdlib.h>

int main(void) {
	
	/* loops indefinitely till syscall error */
	while (1) {
		/* assumes all characters read are alphabets */
		/* block size 100 characters */
		int blockSize = 100;
		char buf[blockSize];
		int numBytesRead = read(STDIN_FILENO, buf, blockSize);
	
		if (numBytesRead == 0) {
			exit(0);
		}
	
		else if (numBytesRead == -1) {
			exit(1);
		}
 
	
		char outBuf[numBytesRead];
		int i;
	
		/* rotate characters based on their ascii values */
		/* stop 1 short to avoid copying EOF */
		for (i = 0; i<numBytesRead-1;i++) {
			if (buf[i]<='M' && buf[i]>='A') {
				outBuf[i] = (char) ((int) buf[i] +13);
			}
			else if (buf[i]>='N' && buf[i]<='Z') {
				outBuf[i] = (char) ((int) buf[i] -13);
			}
			else if (buf[i]>= 'a' && buf[i]<= 'm') {
				outBuf[i] = (char) ((int) buf[i] + 13);
			}
			else if (buf[i] >= 'n' && buf[i] <= 'z') {
				outBuf[i] = (char) ((int) buf[i] - 13);
			}
			else {
				/* not a proper alphabet input */
				exit(1);
			}  
 		}
	
		outBuf[numBytesRead-1] = '\n';
		int numBytesWritten = write(STDOUT_FILENO, outBuf, numBytesRead);
	
		if (numBytesWritten == -1) {
			exit(1);
		}
	}

	return 0; // not reached 
}


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

int main( int argc, char** argv) {
	char pri[2];
	pri[0] = (char)(argc+48);
	pri[1] = '\n';
	write(STDOUT_FILENO,pri,2);
	
	int j;
	/* loops indefinitely till syscall error */
	for (j = 0; j < argc; j++ ){
		pri[0] = (char)(j+48);
		write(STDOUT_FILENO,pri,2);
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
		buf[numBytesRead-1] = '\n';

 		int numBytesWritten = write(STDOUT_FILENO, buf, numBytesRead);
		if ( numBytesWritten == 0 ) {
			exit(0);
		}
		else if ( numBytesWritten == -1 ) {
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
		numBytesWritten = write(STDOUT_FILENO, outBuf, numBytesRead);
	
		if ( numBytesWritten == 0 ) {
			exit(0);
		}
		else if (numBytesWritten == -1) { 
                	exit(1);
                }

		/* a while loop that continues writing until all bytes read
	  	   have been written */
		while (numBytesWritten < numBytesRead) {
			int additionalBytesWritten;
			
			additionalBytesWritten = write(STDOUT_FILENO, 
						outBuf+numBytesWritten, 
						numBytesRead-numBytesWritten);
			numBytesWritten += additionalBytesWritten;
		}
	}

	return 0; // not reached 
}


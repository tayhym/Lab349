/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Matthew Tay mhtay@andrew.cmu.edu
 *          Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 *          Kevin Brennan kbrennan@andrew.cmu.edu
 * Date:    12:00 10/28/2013
 */

#include <unistd.h>
#include <stdlib.h>


int main( int argc, char** argv) {
	int k;
	int blockSize = 20;
	
	char *debugString = "debugging string in rot13.c\n";
	write(STDOUT_FILENO, debugString, 20);

	for ( k = 0; k < argc; k++ ) {
		int numWritten = write(STDOUT_FILENO,argv[k],blockSize);
		if ( numWritten == -1 ){
			exit(1);
		}
		else if ( numWritten == 0 ) {
			exit(0);
		}
	}
	char *debugTwo = "debug Two in rot13.c\n";
	write(STDOUT_FILENO, debugTwo, 25);
	char outbufTwo[blockSize];

	read(STDIN_FILENO, outbufTwo, 15); 
	write(STDOUT_FILENO, debugTwo, 25);
	return 0;
	/* loops indefinitely till syscall error */
	while (1) {
		/* assumes all characters read are alphabets */
		/* block size 100 characters */
		char buf[blockSize];
		char *debugFive = "debug 2.5 in rot13.c\n";
		write(STDOUT_FILENO, debugFive,25);
		int numBytesRead = read(STDIN_FILENO, buf, blockSize);
		char *debugThree = "debug Three in rot13.c\n";
		write(STDOUT_FILENO, debugThree,25);

		if (numBytesRead == 0) {
			exit(0);
		}
		else if (numBytesRead == -1) {
			exit(1);
		}

		char *debugFour = "debug Four in rot13.c\n";
		write(STDOUT_FILENO, debugFour,25);	
		char outBuf[numBytesRead]; //@ remove +1
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
		else if (numBytesWritten == 0) {
			exit(0);
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
	write(STDOUT_FILENO, debugString, 10);
	
	return 0; // not reached 
}


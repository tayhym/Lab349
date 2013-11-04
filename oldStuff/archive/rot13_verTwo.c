/*ROT13 cipher test application
 *
 * Authors: Group Member 1 <email address>
 *          Group Member 2 <email address>
 * Date:    The current time & date
 */

#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
	int i;
	/* loops indefinitely till syscall error */
	while (1) {
		/* assumes all characters read are alphabets */
		/* block size 100 characters */

		/* THE FOLLOWING CODE THAT I COMMENTED OUT IS 
		   FOR PART 1, WHERE YOU READ IN FROM A FILE.*/

		/* int blockSize = 100;
		char buf[blockSize];
		int numBytesRead = read(STDIN_FILENO, buf, blockSize);
	
		if (numBytesRead == 0) {
			exit(0);
		}
	
		else if (numBytesRead == -1) {
			exit(1);
		}*/

 		/* this is for PART TWO where we have to use ROT13 to read 
		   argc and argv from the user stack, print the values on 
		   the screen, and then do the cipher on them */

		/* I am working under the assumption that argc is stored at
		   0xa3000000 and argv[0] is stored at 0xa2fffffc, which is 
		   [sp-4] if [sp] = 0xa3000000.*/

		char* stackPointer = argv[0];
		char buf[argc];
		int numBytesRead = 0;
		int numBytesWritten = 0;
		for (i = 0; i < argc-1; i++);
		{
			buf[i] = *stackPointer;
			stackPointer = (stackPointer - 1);
			numBytesRead++;
		}
		
		/* NEED TO FIGURE OUT HOW TO ACCOUT FOR
		   numBytesRead == -1 CASE BECAUSE I WILL
		   NEVER REACH -1 IN MY LOOP.*/
	
		if (numBytesRead == 0) {
			exit(0);
		}
	
		else if (numBytesRead == -1) {
			exit(1);
		}

		/* Need to print out original contents of 
		   argv before we apply rotation*/

		buf[numBytesRead-1] = '\n';
		numBytesWritten = write(STDOUT_FILENO, buf, numBytesRead);
	
		if (numBytesWritten == -1) {
			exit(1);
		}
	
		char outBuf[numBytesRead];
		int i;
		numBytesWritten = 0;

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
	
		if (numBytesWritten == -1) {
			exit(1);
		}
	}

	return 0; // not reached 
}


/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Matthew Tay mhtay@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 */

#include <unistd.h>
#include <bits/fileno.h>
#include <stdio.h>

#define BLOCKSIZE 100
#define OSTMR_FREQ 3686400

int main(int argc, char** argv)
{
	char buf[BLOCKSIZE];
	unsigned long diff;
	float millis;
	while(1) {
		buf[0] = '>';
		
		write(STDOUT_FILENO,buf,1);
		
		unsigned long startTime = time();
		//int bytesRead = read(STDIN_FILENO,buf,BLOCKSIZE); // co here 
		unsigned long endTime = time();
		//write(STDOUT_FILENO,buf,bytesRead); // co here
		write(STDOUT_FILENO,buf,1);		
		putchar('\n');
		if (endTime < startTime) {
			diff = (UINT32_MAX - startTime) + endTime;
		}
		else {
			diff = endTime - startTime;
		}
		
		millis = ((float)1000)*(((float)diff)/((float)OSTMR_FREQ));
		//printf("%.1fs\n",millis);
		printf("%d ms\n",(int)millis);
	}

	return -1; // Program should never terminate via return
}

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

int main(int argc, char** argv)
{
	char buf[BLOCKSIZE];
	unsigned long diff;
	while(1) {
		buf[0] = '>';
		write(STDOUT_FILENO,buf,1);	// Write prompt
		unsigned long startTime = time();
		int bytesRead = read(STDIN_FILENO,buf,BLOCKSIZE); // Read till enter
		unsigned long endTime = time();
		if ( write(STDOUT_FILENO,buf,bytesRead) < 1 ) return 0; // Echo read characters

		if (endTime < startTime) {
			diff = (UINT32_MAX - startTime) + endTime;
		}
		else {
			diff = endTime - startTime;
		}
		printf("%lu.%lus\n", (diff/100), (diff%100)); //print difference
	}

	return -1; // Program should never terminate via return
}

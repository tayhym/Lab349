/** @file wackAMole.c
 *
 * @brief mole appears at random, and a target to choose is also chosen at random
 *	 
 *
 * Links to libc.
 *
 * @team   Matthew Tay mhtay@andrew.cmu.edu
 * 	   Kevin Brennan k
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdlib.h>

int rand(void);
int main(int argc, char** argv)
{
	int i;
	int BUF_SIZE = 20;
	int SLEEP_TIME = 250; // millis
	char defaultBuffer[BUF_SIZE];
	const char *eraseBuffer = "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";

	for (i = 0; i<BUF_SIZE;i++) {
		if (i%2 == 0) {
			defaultBuffer[i] = '|';
		}
		else {
			defaultBuffer[i] = '-';
		}
	}
	
	write(STDOUT_FILENO, defaultBuffer, BUF_SIZE);
	char moleLetter = 'o';
	char strikeLetter = 'x'; 
	int numPlays = 50;
	while (numPlays>0) {	
		int randomLoc = (rand() % BUF_SIZE);
		if ((randomLoc%2)==0) {
			randomLoc = randomLoc+1 > 18 ? randomLoc-1:randomLoc+1;		
		} 
		defaultBuffer[randomLoc] = moleLetter;
		// refresh screen with moleLetter
		write(STDOUT_FILENO, eraseBuffer, BUF_SIZE);
		write(STDOUT_FILENO, defaultBuffer, BUF_SIZE);		
		sleep(SLEEP_TIME);		
		
		int strikeLoc = (rand()%BUF_SIZE);
		while ((strikeLoc != randomLoc)) {
			if ((strikeLoc%2)!=0) {			
				defaultBuffer[strikeLoc] = strikeLetter;
				// show the guess 
				write(STDOUT_FILENO, eraseBuffer, BUF_SIZE);
				write(STDOUT_FILENO, defaultBuffer, BUF_SIZE);	
	
				sleep(SLEEP_TIME);
				defaultBuffer[strikeLoc] = '-';
				// wrong guess
				write(STDOUT_FILENO, eraseBuffer, BUF_SIZE);
				write(STDOUT_FILENO, defaultBuffer, BUF_SIZE);		
			}
			strikeLoc = rand() % BUF_SIZE;	
			sleep(SLEEP_TIME);
		}
		sleep(SLEEP_TIME);
		defaultBuffer[strikeLoc] = strikeLetter;
		// correct Guess
		write(STDOUT_FILENO, eraseBuffer, BUF_SIZE);
		write(STDOUT_FILENO, defaultBuffer, BUF_SIZE);		

		sleep(SLEEP_TIME);
		defaultBuffer[strikeLoc] = '-';

		// refresh to original 
		write(STDOUT_FILENO, eraseBuffer, BUF_SIZE);
		write(STDOUT_FILENO, defaultBuffer, BUF_SIZE);		
		numPlays--;
	} 		
	return 0x53;
}

#include <stdlib.h>

static unsigned seed[2];

int
rand(void)
{
	seed[0] += 0xa859c317;
	seed[0] += (seed[1] << 13) | (seed[1] >> 19);
	seed[1] += seed[0];
	return seed[0] & 0x7FFFFFFF;
}

void
srand(unsigned new_seed)
{
	seed[0] = seed[1] = new_seed;
}



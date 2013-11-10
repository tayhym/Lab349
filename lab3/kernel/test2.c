/** @file test2.c
 *
 * @brief   This test cases takes an arbitrary value called counter (we have initially
 * 			set this to 100,000) and loops through the while loop for that many
 *			iterations.  During each iteration, the program prints how long it takes
 *			to print letters of the alphabet.
 *
 * Matthew Tay mhtay@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 */

#include <unistd.h>
#include <bits/fileno.h>

int main(int argc, char** argv) {	
  /*
  TEST CASE #1
  */

  printf("TEST CASE 2:\n");

  printf("Prints how long it takes to print alphabet\n");

  int state = 0;
  char buf[2];
  buf[1] = '\b';
  int counter = 0; //determines length of program, can be set to user input
  int letterValue = 65;
  int timeBefore = 0;
  int timeAfter = 0;
  int timeTaken = 0;
  
  while(counter < 100000) {
    if(letterValue > 90)
    {
      letterValue = 65;
    }
	
	timeBefore = time();
    buf[0] = (char)letterValue; // store character
    write(STDOUT_FILENO,buf,1); // write stored character
	timeAfter = time();
	timeTaken = timeAfter - timeBefore;
	printf("Time taken to print letter %c is %d miliseconds.", buf[0], timeTaken);

    buf[0] = '\b';    
    write(STDOUT_FILENO,buf,2);
	letterValue++;
    counter++;
  }
	printf("END OF TEST 2\n");
	return 1; 
}
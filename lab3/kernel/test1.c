/** @file test1.c
 *
 * @brief   This test cases takes an arbitrary value called counter (we have initially
 * 			set this to 100,000) and loops through the while loop for that many
 *			iterations.  During each iteration, the program prints letters of the 
 *			alphabet and sleeps for 100 miliseconds between each character.
 *
 * Matthew Tay mhtay@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 */

#include <unistd.h>
#include <bits/fileno.h>

int main(int argc, char** argv) {	
  /*
  TEST CASES
  */

  printf("TEST CASES\n");
  
  /*
  TEST CASE #1
  */

  printf("TEST CASE 1:\n");

  printf("Prints alphabet for certain time with pause in between\n");

  int state = 0;
  char buf[2];
  buf[1] = '\b';
  int counter = 0; //determines length of program, can be set to user input
  int letterValue = 65;
  
  while(counter < 100000) {
    if(letterValue > 90)
    {
      letterValue = 65;
    }

    buf[0] = (char)letterValue; // store character
    write(STDOUT_FILENO,buf,1); // write stored character
    sleep(100);
    buf[0] = '\b';    
    write(STDOUT_FILENO,buf,2);
	letterValue++;
    counter++;
  }
	printf("END OF TEST 1\n");
	return 1; 
}
  

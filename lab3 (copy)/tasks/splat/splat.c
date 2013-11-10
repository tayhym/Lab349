/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Matthew Tay mhtay@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 */

#include <unistd.h>
#include <bits/fileno.h>

int main(int argc, char** argv) {	
	int state = 0;
	char buf[2];
	buf[1] = '\b';
	while(1) {
		if (state == 0) {
			buf[0] = (char)124; // | character
		}
		else if (state == 1) {
			buf[0] = (char)47; // \ character
		}
		else if (state == 2) {
			buf[0] = (char)45; // - character
		}
		else if (state == 3) {
			buf[0] = (char)92; // / character
		}
		write(STDOUT_FILENO,buf,1); // write stored character
		sleep(200);		    // wait 200ms
		buf[0] = '\b';		
		write(STDOUT_FILENO,buf,2); // erase char written to stdout
	}
	
	return -1; // Program should never terminate via return
}

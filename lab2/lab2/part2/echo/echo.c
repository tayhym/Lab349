/*
 * Matthew Tay
 * Deeptaanshu Kumar
 * Kevin Brennan
 */

#include <stdlib.h>
#include <unistd.h>

int main(void) {
	while(1) {	
		char buf[100];
		int numBytes = read(STDIN_FILENO,buf,100);
		if ( buf[0] == '!' ) {
			exit(0);
		}
		write(STDOUT_FILENO,buf,numBytes);
	}
	return 0;
}
	

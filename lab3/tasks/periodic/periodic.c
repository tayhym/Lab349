/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	int state = 1;
	const char *aString = "A\n";
	const char *bString = "B\n";	
	while (1) {
		if (state == 1) { 
			write(STDOUT_FILENO, aString, sizeof(aString));
			write(STDOUT_FILENO, bString, sizeof(bString));
			sleep(20);
		}
	}
		
	return 0x53;
}

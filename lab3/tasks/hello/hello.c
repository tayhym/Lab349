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
#include <stdarg.h>

unsigned int test[1];

int main(int argc, char** argv)
{
	const char hello[] = "Hello World\r\n";
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	int i =0;
	while(1){
		i++;
	}
	return 0x53;
}

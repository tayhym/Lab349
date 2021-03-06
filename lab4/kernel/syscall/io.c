/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *cbuf __attribute__((unused)), size_t count __attribute__((unused)))
{
  	// initialize useful internal variables
	char *buf = (char *)cbuf;
	int bytesRead = 0;
	char c;	

	// require file descriptor to be stdin
	if (fd != STDIN_FILENO) {
		return -EBADF;
	}
	// Check for valid min bound of cbuf
	else if (!(valid_addr(cbuf, count, RAM_START_ADDR, RAM_END_ADDR))) {
		return -EFAULT;
	}

	while( bytesRead < (int)count ){
		if ( tstc()!=0 ){
			//read a character from stdin 
			c = getc();
			//check for EOT character
			if ((int) c == 4) {
				return bytesRead;
			}
			else if ((int) c == 3) {
				return 0;
			}
			//check for backspace or delete character
			else if (((int) c == 8) || ((int) c == 127)) {
				//remove previous character, if there was a prev character
				if ( bytesRead > 0) {
					bytesRead--; 
					puts("\b \b");
				}
			}
			//check for newline or carriage return 
			else if (((int) c == 10) || ((int) c == 13)) {
				//place newline in the buffer
				*(buf+bytesRead) = '\n';
				putc('\n');
				bytesRead++;
				return bytesRead;
			}

			//write to stdout. function blocks until task completes.
			else {
				*(buf+bytesRead) = c;
				putc(c);
				bytesRead++;
			}
		}
	}
	return bytesRead;
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *cbuf  __attribute__((unused)), size_t count  __attribute__((unused)))
{
	//set useful variables
	const char *buf = (const char *)cbuf;
	size_t bytesWritten = 0;

	//check for bad file descriptor 
	if (fd != STDOUT_FILENO) {
		return -EBADF;
	}
	else if (!(valid_addr(cbuf, count, FLASH_START_ADDR, FLASH_END_ADDR)
				|| valid_addr(cbuf, count, RAM_START_ADDR, RAM_END_ADDR))) {
		return -EFAULT;
	}
	
	while (bytesWritten < count) {
		char c = *(buf+bytesWritten);
		if ( (int)c == 0 || (int)c == 3 || (int)c == 4 ){
			return bytesWritten;
		}
		else {
			putc(c);
			bytesWritten++;
		}
	}
	return bytesWritten;
}


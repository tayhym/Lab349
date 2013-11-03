/*
 * Matthew Tay mhtay@andrew.cmu.edu	
 * Deeptaanshu Kumar deeptaan@andrew.cmu.edu
 * Kevin Brennan kbrennan@andrew.cmu.edu
 */
#include <bits/swi.h> 
#include <c_functions.h> 
#include <bits/errno.h>
#include <bits/fileno.h>
#include <exports.h>
#include <asm_functions.h>

/* internal function prototypes specific to file */ 
void service_SWI_Exit(unsigned int exit_stat);
int service_SWI_Read(unsigned int *regs);
int service_SWI_Write(unsigned int *regs);

extern int exit_status;

/* Called by assembly Swi_Handler, with a swi number and a pointer to
 * register values on the stack. 
 * requires valid swi_Num, this check is done in kernel.
 * returns a return value, depending on which swi_handler was called.
 */
int C_SWI_Handler(int swi_Num, unsigned int *regs) { 
	switch( swi_Num ) {
		case 0x1: service_SWI_Exit((int)regs[0]);
			  return 0;
		case 0x3: return service_SWI_Read(regs);
		case 0x4: return service_SWI_Write(regs);
		default:
			// should not reach here 
			printf("Swi not supported by mini-Kernel");
			return 0xbadc0de;	
	}
}

/* Implements the exit syscall:
 * Takes 1 parameter, the exit status. Exits kernel, returning 
 * exit status to U-Boot. 
 */
void service_SWI_Exit(unsigned int exit_stat) {
	exit_status = exit_stat;
}

/* Implements the read syscall:  
 * Takes 3 parameters: file descriptor for reading, buffer for writing,
 * and the maximum number of bytes to read. 
 * Returns : -EBADF when file descriptor does not match stdin
 * 	     -EFAULT if memory range specified by buffer and its range exceeds
 *	     range of writable memory (SDRAM)
 *   	     Or, return number of characters read into buffer if no error,
 *	     echoing each character from stdin back to stdout till buffer full.	
 *	 			
 */
int service_SWI_Read(unsigned int *regs) {
	int fd = (int) *regs;
	char *buf = (char *) *(regs+1);
	size_t count = (size_t) *(regs+2);
	// initialize useful internal variables
	int bytesRead = 0;
	char c;		
	// require file descriptor to be stdin
	if (fd != STDIN_FILENO) {
		return -EBADF;
	}
	// require that memory range not exceed 64MB  
	if (count>0x10000) {
		return -EFAULT;
	}		
	while (tstc()!=0) {
		//read a character from stdin 
		c = getc();
		//check for EOT character
		if ((int) c == 4) {
			return bytesRead;
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
			putc(c);
			*(buf+bytesRead) = c;
			bytesRead++;
		}
	
	}
	return bytesRead;
}

/* Implements the write syscall: 
 * Takes 3 parameters: file descriptor, buffer for reading,
 *		       number of bytes to write.
 * Returns: -EBADF when file descriptor != stdout. 	
 *	    -EFAULT when memory range specified and max size exceeds readable memory
 *	    Otherwise, writes characters from buffer to stdout util buffer empty and 
 *	    return num characters written. 
 */ 
int service_SWI_Write(unsigned int *regs) {
	//get parameters
	int fd = (int) *regs;
	const char *buf = (const char *) *(regs+1);
	size_t count = (size_t) *(regs+2);
	
	//set useful variables 
	int partOfRom = (int) buf <0x00ffffff; //1 if part of ROM, else part of SDRAM
	size_t bytesWritten = 0;

	//check for bad file descriptor 
	if (fd != STDOUT_FILENO) {
		return -EBADF;
	}
	//check for max size exceeding readable memory when address in ROM
	else if ((partOfRom) && (count>0x4000)) {
		//exceeds 16MB
		return -EFAULT;
	}
	//check for max size exceeding readable memory when address in SDRAM
	else if ((~partOfRom) && (count>0x10000)) {
		//exceeds 64MB
		return -EFAULT;
	} 
	
	else {
		while (bytesWritten < count) {
			putc(*(buf+bytesWritten));
			bytesWritten++;
		}
		return bytesWritten;
	}
}		
	 








#include <bits/swi.h> 
#include <c_functions.h> 
#include <bits/errno.h>
#include <bits/fileno.h>
#include <exports.h>
#include <asm_functions.h>

/* internal function prototypes specific to file */ 
void service_SWI_Exit(unsigned int *regs);
int service_SWI_Read(unsigned int *regs);
int service_SWI_Write(unsigned int *regs);

extern int errNum;
extern int oldsp;

/* Called by assembly Swi_Handler, with a swi number and a pointer to
 * register values on the stack. 
 * requires valid swi_Num, this check is done in kernel.
 * returns a return value, depending on which swi_handler was called.
 */
int C_SWI_Handler(int swi_Num, unsigned int *regs) { 
	printf("C_SWI_HANDLER\n\n");
	printf("swi_Num is %x\n", swi_Num);
	
	printf("regs has value %x\n", regs);
	printf("regs[0] %x\n", regs[0]);
	printf("regs[1] %x\n", regs[1]);
	printf("regs[2] %x\n", regs[2]);
	int retVal;  
	switch(swi_Num) {
		case 0x900001: service_SWI_Exit(regs);
				return errNum;
				break;
		case 0x900003: retVal = service_SWI_Read(regs);
				break;
		case 0x900004: retVal = service_SWI_Write(regs);
				break;
		default:
			// should not reach here 
			printf("Swi not supported by mini-Kernel");
			return 0xbadc0de;	
	}
	puts("back in C_Swi_Handler\n");
	// errno set by service_SWI_Exit(void);
	printf("errNum addr is %x\n", errNum);
	printf("errNum value: %x\n", errNum);
	return errNum;
}

/* Implements the exit syscall:
 * Takes 1 parameter, the exit status. Exits kernel, returning 
 * exit status to U-Boot. 
 */
void service_SWI_Exit(unsigned int *regs) {
	errNum = (int) regs[0];
	printf("jumping to uboot...");
	b_out();
	printf("returned\n");
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
	printf("service_swi_read\n");
	// get parameters from the stack
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
	printf("service_swi_write\n");
	//get parameters
	int fd = (int) regs[0];
	printf("fd is %x\n",fd);
	const char *buf = (const char *) *((unsigned int *)regs+1);
	printf("buf has value %x\n",buf);
	size_t count = (size_t) *((unsigned int*)regs+2);
	printf("count has value %x \n",count);	
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
		printf("exiting C_service_write\n");
		return bytesWritten;
	}
}		
	 








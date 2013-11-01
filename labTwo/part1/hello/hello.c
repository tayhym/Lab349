
/* Tests some useful export functions from uboot */


#include<exports.h>
#include<bits/swi.h>
#include<bits/errno.h>
#include <bits/fileno.h>
extern int write(unsigned int fd, const char *buf, size_t count); 
int main(void) {
	puts("Hello World!\n");
	// put some characters on stdout
	putc('h');
	putc('\n');
	putc('i');
	
	char *strMsg = "Enter a char: ";
	puts(strMsg);
	char c = getc();
	char *resMsg = "You entered: ";
	puts(resMsg);
	putc(c);
	putc('\n');
	write(STDOUT_FILENO,"hello from hello.c",10);
	return 0;
}
	


/* Tests some useful export functions from uboot */


#include<exports.h>
#include<bits/swi.h>
#include<bits/errno.h>
#include <bits/fileno.h>
#include <sys/types.h>

extern int write(unsigned int fd, const char *buf, size_t count);

int main(void) {
	puts("Hello World!\n");
	write(STDOUT_FILENO, "helloFrom hello.c!\n",18);
	
	//write(STDOUT_FILENO,"Seven!!",7);
	return 0;
}
	

/* Tests some useful export functions from uboot */


#include<exports.h>

int main(void) {
	// put some characters on stdout
	putc('h');
	putc('i');
	
	char *strMsg = "Enter a char: ";
	puts(strMsg);
	char c = getc();
	char *resMsg = "You entered: ";
	puts(resMsg);
	puts(c);
	puts('\n');
	return 0;
}
	

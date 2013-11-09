/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */

#include <unistd.h>
#include <bits/fileno.h>

int main(int argc, char** argv) {	
	int state = 0;
	char buf[2];
	buf[1] = '\b';
	while(1) {
		if (state == 0) {
			buf[0] = (char)124;
		}
		else if (state == 1) {
			buf[0] = (char)47;
		}
		else if (state == 2) {
			buf[0] = (char)45;
		}
		else if (state == 3) {
			buf[0] = (char)92;
		}
		write(STDOUT_FILENO,buf,1);		
		sleep(200);
		buf[0] = '\b';
		write(STDOUT_FILENO,buf,2);
	}
	
	return -1;
}

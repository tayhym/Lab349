#! bin/bash 
#

set architecture armv5te 
target remote localhost:1234
add-symbol-file ~/Documents/18-349/lab349/lab3/kernel/kernel 0xa3000000
add-symbol-file ~/Documents/18-349/lab349/lab3/tasks/hello/package.bin 0xa0000000



#! bin/bash 
#

set architecture armv5te 
target remote localhost:1234
add-symbol-file kernel 0xa3000000
 add-symbol-file ~/Documents/18-349/lab349/lab3/tasks/bin/rot13 0xa000000

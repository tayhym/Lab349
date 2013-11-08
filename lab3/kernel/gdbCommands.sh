#! bin/bash 
#

set architecture armv5te 
target remote localhost:1234
add-symbol-file kernel 0xa3000000

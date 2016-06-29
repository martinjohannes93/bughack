#Usage
1. Compile<br />
gcc -o test test.c<br />
2. Run multiply times<br />
./test<br />

Is the adress of target the same? ==> ASLR is off
Else ==> ASLR is on

This simple test program verifies that ASLR works probably or is disabled probably

You can also check the config:<br />
cat /proc/sys/kernel/randomize_va_space

Set this file to "0" to turn ASLR off ;)

#include <stdio.h>
#include <stdlib.h>

/*
On the target/hacked system, you should turn off ASLR

Command to turn ASLR off:
sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"

Command to turn ASLR on, again (by default it is enabled):
sudo sh -c "echo 1 > /proc/sys/kernel/randomize_va_space"
*/

int main(void) {
	unsigned int target = 0x41414141;
	printf("address of target: %x \n", &target);

	return 0;
}

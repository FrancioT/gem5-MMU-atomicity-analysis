#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <gem5/m5ops.h>


int main()
{
	m5_checkpoint(0, 0);
	m5_reset_stats(0, 0);
	
	int *val1 = malloc(sizeof(int));
	int *val2 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	
	*val1 = 0;
	*val2 = 0;
	
	m5_dump_stats(0, 0);
	m5_exit(0);
	return 0;
}




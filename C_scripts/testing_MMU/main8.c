//#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <gem5/m5ops.h>
#include <unistd.h>


int main()
{
	m5_checkpoint(0, 0);
	m5_reset_stats(0, 0);

	int status=0;
	int *val1 = malloc(sizeof(int));
	int *val2 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	
	pid_t pid = fork();
	if(pid<0)
	{
//		printf("ERROR in forking!\n");
		return 1;
	}
	else if(pid == 0)
	{
//		printf("Child begin\n");
		*val1 = 0;
		*val2 = 0;
//		printf("Child finished\n");
	}
	else
	{
//		printf("Parent begin\n");
		if(wait(&status)!=pid) return -1;
		if(status!=0) return -1;
		*val1 = 1;
		*val2 = 1;
//		printf("Parent finished\n");
		m5_dump_stats(0, 0);
		m5_exit(0);
	}
	return 0;
}



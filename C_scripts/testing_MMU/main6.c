#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <gem5/m5ops.h>
#define SIZE 2048


void func(int* ptr)
{
	int* myPtr = malloc(SIZE*sizeof(int));
	for(int i=0; i<10; i++)
		ptr[i] = 1;
	for(int i=0; i<SIZE; i++)
	{
		myPtr[i] = ptr[i];
	}
	free(myPtr);
}

int main()
{
	m5_checkpoint(0, 0);
	m5_reset_stats(0, 0);
	printf("Inizio");
	fflush(stdout);
	// alloco 2 pagine (assumendo che una pagina abbia dimensione 4096 byte) 
	int *ptr = mmap(NULL, SIZE*sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

	if(ptr == MAP_FAILED)
	{
		printf("ERROR Mapping Failed\n");
		return 1;
	}

	for(int i=0; i<SIZE; i++)
		ptr[i] = i;
	
	func(ptr);
	
	int err = munmap(ptr, SIZE*sizeof(int));
	if(err != 0)
	{
		printf("ERROR UnMapping Failed\n");
		return 1;
	}
	printf("\n\n");
	
	m5_dump_stats(0, 0);
	m5_exit(0);
	return 0;
}




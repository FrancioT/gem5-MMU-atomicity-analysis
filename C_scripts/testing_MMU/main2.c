#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#define SIZE 2048

//spinlock_t my_lock=SPIN_LOCK_UNLOCKED;

void parent_func(int* ptr)
{
	int* myPtr = malloc(SIZE*sizeof(int));
	for(int i=0; i<10; i++)
		ptr[i] = 1;
	for(int i=0; i<SIZE; i++)
	{
		myPtr[i] = ptr[i];
	}
	
	//while(!spin_trylock(&my_lock));
	//printf("Parent acquired lock");
	for(int i=0; i<SIZE; i++)
	{
		if(i==1024)
			printf("\n");
		printf("P:%d\t", myPtr[i]);
	}
	//spin_unlock(&my_lock);
	
	free(myPtr);
}

void child_func(int* ptr)
{
	int* myPtr = malloc(SIZE*sizeof(int));
	for(int i=0; i<10; i++)
		ptr[i] = 0;
	for(int i=0; i<SIZE; i++)
	{
		myPtr[i] = ptr[i];
	}
	
	//while(!spin_trylock(&my_lock));
	//printf("Child acquired lock");
	for(int i=0; i<SIZE; i++)
	{
		if(i==1024)
			printf("\n");
		printf("C:%d\t", myPtr[i]);
	}
	//spin_unlock(&my_lock);
	
	free(myPtr);
}

int main()
{
	// alloco 2 pagine (assumendo che una pagina abbia dimensione 4096 byte) 
	int *ptr = mmap (NULL, SIZE*sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

	if(ptr == MAP_FAILED)
	{
		printf("ERROR Mapping Failed\n");
		return 1;
	}

	for(int i=0; i<SIZE; i++)
		ptr[i] = i;

	for(int i=0; i<SIZE; i++)
	{
		if(i==1024)
			printf("\n");
		printf("%d\t",ptr[i]);
	}
	printf("\n\n");
	
	pid_t p = fork();
	if(p<0)
	{
		printf("ERROR in forking!");
		return 1;
	}
	else if(p == 0)
		child_func(ptr);
	else
		parent_func(ptr);
	
	int err = munmap(ptr, SIZE*sizeof(int));
	if(err != 0)
	{
		printf("ERROR UnMapping Failed\n");
		return 1;
	}
	printf("\n\n");
	return 0;
}




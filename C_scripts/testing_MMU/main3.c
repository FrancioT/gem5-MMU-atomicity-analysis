#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
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
	
	for(int i=0; i<SIZE; i++)
	{
		if(i==1024)
			printf("\n");
		printf("P:%d\t", myPtr[i]);
	}
	
	free(myPtr);
}

int main()
{
	// alloco 2 pagine (assumendo che una pagina abbia dimensione 4096 byte) 
	int *ptr = mmap(NULL, SIZE*sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

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
	
	func(ptr);
	
	int err = munmap(ptr, SIZE*sizeof(int));
	if(err != 0)
	{
		printf("ERROR UnMapping Failed\n");
		return 1;
	}
	printf("\n\n");
	
	ptr = mmap(NULL, 3*sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	// NON FUNZIONA:
	// usando MAP_HUGETLB sto allocando pagine molto grosse, quindi sto usando il tlb di livello 2 (l2 shared) che è più grande rispetto a l1, ma più lento
	//ptr = mmap(NULL, (SIZE*2)*64, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, 0, 0);
	if(ptr == MAP_FAILED)
	{
		printf("ERROR Mapping Failed\n");
		return 1;
	}
	printf("%d %d %d\n", ptr[0], ptr[1], ptr[2]);
	munmap(ptr, 3*sizeof(int));
	//system("ls");
	return 0;
}




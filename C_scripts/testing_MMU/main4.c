#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#define PAGE_SIZE 4096
#define PAGE_NUM 100

extern void data_test(void *buf, int page_cnt, int op_cnt);

int main()
{
	// alloco 2 pagine (assumendo che una pagina abbia dimensione 4096 byte) 
	int *ptr = mmap(NULL, PAGE_SIZE*PAGE_NUM, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

	if(ptr == MAP_FAILED)
	{
		printf("ERROR Mapping Failed\n");
		return 1;
	}

	data_test(ptr, PAGE_NUM, 0xFFFF);
	
	int err = munmap(ptr, PAGE_SIZE*PAGE_NUM);
	if(err != 0)
	{
		printf("ERROR UnMapping Failed\n");
		return 1;
	}
	printf("\n\n");
	return 0;
}




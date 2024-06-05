#include <stdio.h>
#include <pthread.h>
#define DIM 1000000L

unsigned int prec_elements[DIM] = {0};
unsigned int my_values[DIM] = {0};
unsigned int i = 0;

void *producer(void *par)
{
	unsigned int n_times = *((unsigned int *)par);
	for(i=0; i<n_times; i++)
	{
		prec_elements[i] = i+1;
		my_values[i] = i+1;
	}
	return NULL;
}

void *consumer(void *par)
{
	unsigned int n_times = *((unsigned int *)par);
	unsigned int counter=0;
	unsigned int prec = -1;
	unsigned int val = -1;
	unsigned int index = 0;
	index = i;
	while(index < n_times)
	{
		val = my_values[index];
		prec = prec_elements[index];
		if(val > prec)
		{
			++counter;
			printf("prec_element  |  my_index:  %lld  |  %lld\n", prec, val);
		}
		index = i;
	}
	printf("Number of memory order inconsistency: %lld\n", counter);
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	unsigned int param = DIM;

	pthread_create(&thread_1, NULL, producer, (void *)&param);
	pthread_create(&thread_2, NULL, consumer, (void *)&param);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	return 0;
}



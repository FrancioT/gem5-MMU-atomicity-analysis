#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#define DIM 1000000L

atomic_uint prec_elements[DIM];
atomic_uint my_values[DIM];
atomic_uint i;

void *producer(void *par)
{
	unsigned int n_times = *((unsigned int *)par);
	for(atomic_store(&i, 0); atomic_load(&i)<n_times; atomic_fetch_add(&i, 1))
	{
		atomic_store(&prec_elements[atomic_load(&i)], atomic_load(&i)+1);
		atomic_store(&my_values[atomic_load(&i)], atomic_load(&i)+1);
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
	index = atomic_load(&i);
	while(index < n_times)
	{
		val = atomic_load(&my_values[index]);
		prec = atomic_load(&prec_elements[index]);
		if(val > prec)
		{
			++counter;
			printf("prec_element  |  my_index:  %u  |  %u\n", prec, val);
		}
		index = atomic_load(&i);
	}
	printf("Number of memory order inconsistency: %u\n", counter);
	return NULL;
}

int main()
{
	pthread_t thread_1, thread_2;
	unsigned int param = DIM;
	
	for(int j=0; j<DIM; j++)
	{
		atomic_init(&my_values[j], 0);
		atomic_init(&prec_elements[j], 0);
	}
	atomic_init(&i, 0);
	pthread_create(&thread_1, NULL, producer, (void *)&param);
	pthread_create(&thread_2, NULL, consumer, (void *)&param);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	
	return 0;
}



// https://preshing.com/20120913/acquire-and-release-semantics/
// https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/
// https://en.cppreference.com/w/cpp/atomic/memory_order
//
// N.B: da notare il tempo di esecutione del programma con il memory order corretto e senza
// time ./no_acquire_release
// time ./with_acquire_release
//
// N.B 2: non posso usare il vettore atomic anche togliendo memory order acquire e release, 
//        perchè nella trduzione in assembly mi mette comunque la ldar e stlr per arm e mi traduce con le fence in riscv
// raspberry: pp1o=0lo19#3j

#include <stdio.h>
#include <atomic>
#include <pthread.h>
//#include <gem5/m5ops.h>
#define DIM 100000000L
//#define DIM 50000L

using namespace std;
long long prec_elements[DIM] = {0};
//atomic<long long> My_values[DIM] = {0};
long long my_values[DIM] = {0};
long long i = 0;

void *producer(void *par)
{
	long long n_times = *((long long *)par);
	for(i=0; i<n_times; i++)
	{
		prec_elements[i] = i+1;
		//My_values[i].store(i+1, memory_order_release);
		my_values[i] = i+1;
	}
	return NULL;
}

void *consumer(void *par)
{
	long long n_times = *((long long *)par);
	long long counter=0;
	long long prec = -1;
	long long val = -1;
	long long index = 0;
	index = i;
	while(index < n_times)
	{
		val = my_values[index];
		//val = My_values[index].load(memory_order_acquire);
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
	long long param = DIM;

//	m5_switch_cpu();	
	pthread_create(&thread_1, NULL, producer, (void *)&param);
	pthread_create(&thread_2, NULL, consumer, (void *)&param);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	fflush(stdout);
//	m5_exit(0);
	return 0;
}

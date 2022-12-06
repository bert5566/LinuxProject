#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#define MEMORY_SIZE 1000000


void print_result( int *result , char * str) {
	for(int i=0; i<MEMORY_SIZE; i+=2) {
		if(result[i+1] != 0){
			printf(" %s Virtual Address: 0x%x  Physical Address: 0x%x\n", str , result[i], result[i+1]);
		}
	}
}

void* child(void* arg){
	int* result = (int *)arg;
	syscall(548,"child thread",result);
	pthread_exit(NULL);
};
int main ()
{
	int* result = calloc(MEMORY_SIZE , sizeof(int));
	int* result_1 = calloc(MEMORY_SIZE , sizeof(int));
	int* result_2 = calloc(MEMORY_SIZE , sizeof(int));
	pthread_t thread1 , thread2;

	syscall(548,"Main thread is me"); //main thread 
	pthread_create(&thread1,NULL,child,(void *)result_1);
	pthread_create(&thread2,NULL,child,(void *)result_2);

	syscall(548,"Main thread",result);
	print_result(result , "main thread");
	pthread_join(thread1,NULL);
	print_result(result_1 , "thread1");
	pthread_join(thread2,NULL);
	print_result(result_2 , "thread2");

    return 0;	
}

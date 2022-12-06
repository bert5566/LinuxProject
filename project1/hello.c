#include <stdio.h>
#include<pthread.h>
#define NUM_THREADS 2

void *read_data_segement(void *param);
void *read_bss_segement(void *param);
void *read_stack_segement(void *param);
void *read_heap_segement(void *param);
void *read_code_segement(void *param);
int count = 0;

int segement_data = 1;        // 儲存於 data 區
int segement_bss;            // 儲存於 bss 區段
int main() {
	int segement_stack = 1;              // 儲存於 stack 區段
	int *segement_heap = malloc(sizeof(int) * 100); // 儲存於 heap 區段
	// ...
  	free(segement_heap);

	int i;
	pthread_t tid[NUM_THREADS];

	for(i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tid[i], NULL, read_data_segement, &segement_data);
	}
	for(i = 0; i < NUM_THREADS; i++) {
		count = 0;
		pthread_join(tid[i], NULL);
	}

	for(i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tid[i], NULL, read_bss_segement, &segement_bss);
	}
	for(i = 0; i < NUM_THREADS; i++) {
		count = 0;
		pthread_join(tid[i], NULL);
	}
	for(i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tid[i], NULL, read_stack_segement, &segement_stack);
	}
	for(i = 0; i < NUM_THREADS; i++) {
		count = 0;
		pthread_join(tid[i], NULL);
	}
	for(i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tid[i], NULL, read_heap_segement, &segement_heap);
	}
	for(i = 0; i < NUM_THREADS; i++) {
		count = 0;
		pthread_join(tid[i], NULL);
	}

	for(i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tid[i], NULL, read_code_segement, &main);
	}
	for(i = 0; i < NUM_THREADS; i++) {
		count = 0;
		pthread_join(tid[i], NULL);
	}

  	return 0;
};

void *read_data_segement(void *param) {
	printf("thread_%d\n" , ++count);
    int *p = (int*)param;
	printf("data segement vitualMemory   : %p\n" ,p);
	syscall(548,p);
	printf("data segement PhysicalMemory : %p\n" ,*p);
    pthread_exit(NULL);
}

void *read_bss_segement(void *param) {
	printf("thread_%d\n" , ++count);
	int *p = (int*)param;
	printf("bss segement  vitualMemory   : %p\n" ,p);
	syscall(548,p);
	printf("bss segement  PhysicalMemory : %p\n" ,*p);
	pthread_exit(NULL);
}

void *read_stack_segement(void *param) {
	printf("thread_%d\n" , ++count);
	int *p = (int*)param;
	printf("stack segement  vitualMemory   : %p\n" ,p);
	syscall(548,p);
	printf("stack segement  PhysicalMemory : %p\n" ,*p);
	pthread_exit(NULL);
}

void *read_heap_segement(void *param) {
	printf("thread_%d\n" , ++count);
	int *p = (int*)param;
	printf("heap segement  vitualMemory	  : %p\n" ,p);
	syscall(548,p);
	printf("heap segement  PhysicalMemory : %p\n" ,*p);
	pthread_exit(NULL);
}

void *read_code_segement(void *param) {
	printf("thread_%d\n" , ++count);
	int *p = (int*)param;
	printf("code segement  vitualMemory	  : %p\n" ,p);
	syscall(548,p);
	printf("code segement  PhysicalMemory : %p\n" ,*p);
	pthread_exit(NULL);
}
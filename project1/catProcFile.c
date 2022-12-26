#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

void *thread_func(void *arg) {
	printf("Thread function (TID %d) running at virtual address: %p\n", gettid(), &thread_func);

	char line[256];
	FILE *fp;
	char path[32];

	snprintf(path, sizeof(path), "/proc/%d/maps", gettid());
	fp = fopen(path, "r");
	if (fp == NULL) {
		printf("Error opening %s\n", path);
		return NULL;
	}

	printf("Physical addresses of thread function:\n");

	while (fgets(line, sizeof(line), fp)!= NULL) {
		printf("%s", line);
	}

	fclose(fp);
	return NULL;
}

int main() {
	pthread_t thread;
	int rc;

	printf("Main function (TID %d) running at virtual address: %p\n", gettid(), &main);

	rc = pthread_create(&thread, NULL, thread_func, NULL);
	if (rc != 0) {
		printf("Error creating thread\n");
		return 1;
	}

	pthread_join(thread, NULL);

	return 0;
}
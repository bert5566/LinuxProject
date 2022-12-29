#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define MEMORY_SIZE 1000000
#define gettid() syscall(SYS_gettid)

void thread_func() {
	char line[256];
	FILE *fp;
	char path[32];
	printf("gettid() %d \n" , gettid());
	snprintf(path, sizeof(path), "/proc/%d/maps", gettid());
	fp = fopen(path, "r");
	if (fp == NULL) {
		printf("Error opening %s\n", path);
		return NULL;
	}

	printf("Physical addresses of thread function:\n");

	while (fgets(line, sizeof(line), fp)!= NULL) {
		if (strstr(line, "[heap]")) {
			printf("Heap segment: %s", line);
		} else if (strstr(line, "[stack]")) {
			printf("Stack segment: %s", line);
		} else if (strstr(line, "[vdso]")) {
			printf("Virtual dynamic shared object segment: %s", line);
		} else if (strstr(line, "[vsyscall]")) {
			printf("Virtual system call segment: %s", line);
		} else if (strstr(line, "[vvar]")) {
			printf("Virtual variable segment: %s", line);
		} else if (strstr(line, "/")) {
			printf("Mapped file segment: %s", line);
		} else {
			printf("Other segment: %s", line);
		}
	}

	fclose(fp);
	return NULL;
}

int main() {
	printf("I am the child process (PID %ld)\n", (long) getpid());
	int* result = calloc(MEMORY_SIZE , sizeof(int));
	thread_func();
	syscall(548,result);
	return 0;
}
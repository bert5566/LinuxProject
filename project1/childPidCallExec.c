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
	pid_t pid;
	int* result = calloc(MEMORY_SIZE , sizeof(int));
	char *args[] = {"./test3" , NULL};

	pid = fork();
	switch(pid) {
		case -1:
			perror("fork");
			return 1;
		case 0:
			// Child proces
			//printf("I am the child process (PID %ld)\n", (long) getpid());
			// 在子進程中執行ls命令
			//execl("/bin/echo","echo","executed by execl" ,NULL);
			execvp(args[0] , args);

			//thread_func();
			//syscall(548,result);
			break;
		default:		
			// This is the parent process.
			printf("I am the parent process (PID %ld)\n", (long) getpid());
			thread_func();
			syscall(548,result);
			wait(NULL);
			printf("Child process with ID %d terminated\n", pid);
			break;
	};

	return 0;
}
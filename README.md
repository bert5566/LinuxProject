# Linux kernel
steps ============ <br>
1. 建立System call 
	1. mkdir /hello
	2. Makefile
2. cd .. 回上一層
3. vim Makefile
	1. 找出 cory-y += kernel.... 最後加上 /hello (大約在1073行)
4. vim include/linux/syscalls.h
	1. 最後一行加上 asmlinkage long sys_helloworld(void);
5. vim arch/x86/entry/syscalls/syscall_64.tbl
	1. 最後一行加上 common hello sys_hello
6. make menuconfig (預設)
7. sudo make
8. make modules_install install
9. sudo update gru
10. reboot

## mmstruct <br>
![image](https://user-images.githubusercontent.com/49525437/201469043-8e69a072-429e-4c8f-8ff5-09887a3412b5.png) <br>
[start_code,end_code) : 表示code 地址 <br>
[start_data,end_start) : 表示data地址 <br>
[start_brk,brk) : 分别表示heap段的起始空間 和當前的heap指針。 <br>
[start_stack,end_stack) : 表示stack 的地址空間範圍。 <br>


```C
	struct mm_struct * mm ;
	mm->start_code;
```

	
	

# Linux kernel
steps ============ <br>
1. 建立System call 
	1. mkdir /hello
	2. Makefile
2. cd .. 回上一層
3. vim Makefile
	1. 找出 cory-y += kernel.... 最後加上 /hello (大約在1073行)
4. vim include/linux/syscalls.h
	1. 最後一行加上 asmlinkage long sys_hello(void);
5. vim arch/x86/entry/syscalls/syscall_64.tbl
	1. 最後一行加上 common hello sys_hello
6. make menuconfig (預設)
7. sudo make
8. make modules_install install
```C
合併指令
-sudo make -j && sudo make -j modules_install && sudo make -j install
```
10. sudo update grub
11. reboot

## mmstruct <br>
![image](https://user-images.githubusercontent.com/49525437/201509271-72ed68cf-449f-46d5-88c8-2bb31e4bcbba.png)<br>
[start_code,end_code) : 表示code 地址 <br>
[start_data,end_start) : 表示data地址 <br>
[start_brk,brk) : 分别表示heap段的起始空間 和當前的heap指針。 <br>
[start_stack,end_stack) : 表示stack 的地址空間範圍。 <br>


```C
	struct mm_struct * mm ;
	mm->start_code;
```

Linux用task_struct來描述一個process，透過task_struct->mm->mmap可以找到vm_area_struct，linux用利用VMA(virtual memory area)來描述每個segment特性，大小

kernel頁碼轉換 https://zhuanlan.zhihu.com/p/436879901


### System call 傳遞參數
https://biscuitos.github.io/blog/SYSCALL_PARAMENTER_STRINGS/

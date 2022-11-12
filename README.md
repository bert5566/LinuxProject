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

mmstruct
![image](https://user-images.githubusercontent.com/49525437/201469043-8e69a072-429e-4c8f-8ff5-09887a3412b5.png)


Linux kernel
============
steps
1. 建立System call
	1. mkdir /hello
	2. Makefile
2. cd .. 回上一層
3. vim Makefile
	3.1 找出 cory-y += kernel.... 最後加上 /hello (大約在1073行)
4. vim include/linux/syscalls.h
	4.1 最後一行加上 asmlinkage long sys_helloworld(void);
5. vim arch/x86/entry/syscalls/syscall_64.tbl
	5.1 最後一行加上 common hello sys_hello
6. make menuconfig (預設)
7. sudo make
8. make modules_install install
9. sudo update grub
10.reboot

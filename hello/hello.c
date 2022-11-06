#include<linux/kernel.h>

asmlinkage long sys_hello(void)
{
	printk("hello from my sys call\n");
	return 0;
}

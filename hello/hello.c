#include<linux/sched.h>
#include <linux/fs_struct.h>
#include<linux/kernel.h>


asmlinkage long sys_hello(int __user *result)
{
	
	struct mm_struct *mm;
	struct vm_area_struct *vma;

	printk(KERN_INFO "Hello mysyscall\n");
	printk(KERN_INFO "Process name is %s (pid %d)\n", current->comm, current->pid);

	mm = current->mm;
//	vma = find_vma(mm, mm->start_data);
//printk("mm->start_data(%p) vma->vm_start(%p) mm->end_data(%p) vma->vm_end(%p)\n", (uvp_t)mm->start_data, (uvp_t)vma->vm_start, (uvp_t)mm->end_data, (uvp_t)vma->vm_end);
//	printk(KERN_INFO "Code  Segment start = 0x%lx, end = 0x%lx \n",current->mm->start_code, current->mm->end_code);
//	printk(KERN_INFO "Data  Segment start = 0x%lx, end = 0x%lx\n" ,  mm->start_data, mm->end_data);
//	printk(KERN_INFO "Stack Segment start = 0x%lx\n",mm->start_stack);
		
	return 0;
	
}

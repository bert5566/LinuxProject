#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/syscalls.h>
#include <linux/highmem.h>
#include <linux/hugetlb.h>
#include <linux/mm.h>
unsigned long virtophys(struct mm_struct *mm, unsigned int virt) {

	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long phys;

	pgd = pgd_offset(mm, virt);
	if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
		goto out;

	p4d = p4d_offset(pgd, virt);
	if (p4d_none(*p4d) || unlikely(p4d_bad(*p4d)))
		goto out;

	pud = pud_offset(p4d, virt);
	if (pud_none(*pud) || unlikely(pud_bad(*pud)))
		goto out;

	pmd = pmd_offset(pud, virt);

	if (pmd_huge(*pmd))
		goto out;

	if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
		goto out;

	pte = pte_offset_map(pmd, virt);
	if (!pte_present(*pte)) {
		pte_unmap(pte);
		goto out;
	}

	phys = pte_val(*pte) & PAGE_MASK;
	pte_unmap(pte);

	return phys;
out:
	return 0;
}


SYSCALL_DEFINE1(hello, int __user *, result)
{
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	unsigned int virt, phys;

	printk(KERN_INFO "Hello mysyscall\n");
	printk(KERN_INFO "Process name is %s (pid %d)\n", current->comm, current->pid);

	if(current->active_mm){
		printk(KERN_INFO "current->active_mm is exist");
		printk(KERN_INFO "Code segment start=0x%d , end=0x%d\n" , current->active_mm->start_code , current->active_mm->end_code);	
		printk("Data segment start=0x%d , end=0x%d\n" , current->active_mm->start_data , current->active_mm->end_data);
		printk("Stack segment start=0x%d" , mm->start_stack);
	};

	if(current->mm){
		printk(KERN_INFO "current->mm is exist");
		printk(KERN_INFO "Code segment start=0x%d , end=0x%d\n" , current->mm->start_code , current->mm->end_code);
		printk("Data segment start=0x%d , end=0x%d\n" , mm->start_data , mm->end_data);
		printk("Stack segment start=0x%d" , mm->start_stack);
		mm = current->mm;
		if (mm && mm->mmap) {
			for (vma = mm->mmap; vma; vma = vma->vm_next) {
				for (virt = vma->vm_start; virt < vma->vm_end; virt += PAGE_SIZE) {
					phys = virtophys(mm, virt);
					printk("phyAddress %d\n" , phys);
				}
			}
		}
	}
	return 0;
};

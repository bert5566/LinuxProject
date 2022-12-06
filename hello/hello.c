#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/syscalls.h>
#include <linux/highmem.h>
#include <linux/hugetlb.h>

unsigned long virtophys(unsigned int virt) {

	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long phys;

	pgd = pgd_offset(current->mm, virt);
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


asmlinkage long sys_hello(int __user *result)
{
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	long start_phy , end_phy;


	if(current->mm){
		mm = current->mm;
		printk(KERN_INFO "Hello mysyscall\n");
		printk(KERN_INFO "Process name is %s (pid %d)\n", current->comm, current->pid);
		
		if (mm && mm->mmap) {
			int i = 0;
			for (vma = mm->mmap; vma; vma = vma->vm_next) {
					++i;
					printk(KERN_INFO , "%d\n" , i); 
					printk(KERN_INFO , "vma start = 0x%d\n" , vma->vm_start); 
					printk(KERN_INFO , "vma end = 0x%d\n" , vma->vm_end); 
			};

			start_phy = virtophys(mm->start_data);
			end_phy = virtophys(mm->end_data);
			printk(KERN_INFO "Data segment virtual start=0x%d , end=0x%d\n" , mm->start_data , mm->end_data);
			printk(KERN_INFO "Data segment physical start=0x%d , end=0x%d\n" , start_phy , end_phy);


			start_phy = virtophys(mm->start_stack);
			printk(KERN_INFO "Stack segment virtual start=0x%d \n" ,mm->start_stack );
			printk(KERN_INFO "Stack segment physical start=0x%d \n" , start_phy );

			start_phy = virtophys(mm->start_brk);
			printk(KERN_INFO "Heap segment virtual start=0x%d \n" ,mm->start_brk);
			printk(KERN_INFO "Heap segment physical start=0x%d \n" , start_phy );


			start_phy = virtophys(mm->start_code);
			end_phy = virtophys(mm->end_code);
			printk(KERN_INFO "Code segment virtual start=0x%d , end=0x%d\n" , mm->start_code , mm->end_code);
			printk(KERN_INFO "Code segment physical start=0x%d , end=0x%d\n" , start_phy , end_phy);
		};
	};

	long physicalAddress = virtophys(result);
	if (copy_to_user(result, &physicalAddress, sizeof(physicalAddress))) {
		return -1;
	};
	return 0;
}

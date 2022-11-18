#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/syscalls.h>
#include <linux/highmem.h>
#include <linux/hugetlb.h>

unsigned long virtophys(struct mm_struct *mm, unsigned int virt) {

	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long phys;

	pgd = pgd_offset(mm, virt);//根據入參記憶體描述符mm和虛擬地址address，找到address在頁全域性目錄中相應表項的線性地址。
	if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
		goto out;

	p4d = p4d_offset(pgd, virt);//根據入參pgd和虛擬地址address，找到address在頁四級目錄中相應表項的線性地址。
	if (p4d_none(*p4d) || unlikely(p4d_bad(*p4d)))
		goto out;

	pud = pud_offset(p4d, virt);//根據入參p4d和虛擬地址address，找到address在頁上級目錄中相應表項的線性地址。
	if (pud_none(*pud) || unlikely(pud_bad(*pud)))
		goto out;

	pmd = pmd_offset(pud, virt);//根據入參pud和虛擬地址address，找到address在頁中間目錄中相應表項的線性地址。

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

asmlinkage long sys_hello(char *const_string)
{
	char kconst_string[128];
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	unsigned int virt, phys;

	if(current->mm){
		mm = current->mm;
		if (copy_from_user(kconst_string , const_string ,128)){
			printk("const_string copy error");
			return -EINVAL;
		}
		printk(KERN_INFO "kernel-const_string: %s\n" , kconst_string);
		printk(KERN_INFO "Process name is %s (pid %d)\n", current->comm, current->pid);
		printk(KERN_INFO "current->mm is exist");
		printk(KERN_INFO "Code segment start=0x%d , end=0x%d\n" , mm->start_code , mm->end_code);
		printk(KERN_INFO "Data segment start=0x%d , end=0x%d\n" , mm->start_data , mm->end_data);
		printk(KERN_INFO "Stack segment start=0x%d" ,mm->start_stack);

		printk(KERN_INFO "How many thread share %d" ,mm->mm_users);
		if (mm && mm->mmap) {
			for (vma = mm->mmap; vma; vma = vma->vm_next) {
					printk(KERN_INFO "vma vm_start 0x%d" , vma->vm_start);
					for (virt = vma->vm_start; virt < vma->vm_end; virt += PAGE_SIZE) {
						phys = virtophys(mm, virt);
					}
					printk(KERN_INFO "vma vm_end 0x%d" , vma->vm_end);
				
			}
		}
	}
	return 0;
};

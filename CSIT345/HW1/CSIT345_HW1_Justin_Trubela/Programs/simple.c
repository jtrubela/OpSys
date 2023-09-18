/**
 * simple.c
 *
 * A simple kernel module.
 *
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/jiffies.h>
// #include <linux/seconds.h>
#include <linux/timer.h>


unsigned long jiffs_Start, jiffs_End;
unsigned long timeElapsed;

/* This function is called when the module is loaded. */
int simple_init(void)
{
    jiffs_Start = jiffies;
    
    printk(KERN_INFO "Loading Module\n");
    printk(KERN_INFO "[Module Start Jiffies: %lu] \n", jiffs_Start);
    printk(KERN_INFO "Golden Ratio is: %lu \n", GOLDEN_RATIO_PRIME);
    printk(KERN_INFO "Number of timer interupts[start]: %lu \n", jiffies);
    printk(KERN_INFO "Values of HZ: %d\n", HZ);
    
    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
    printk(KERN_INFO "Removing Module\n");
    
    //get number of interupts at end
    jiffs_End = jiffies;
    
    
    //printk(KERN_INFO "\n(Module Removed End Time: %lu]", jiffs_End);
    timeElapsed = jiffs_End - jiffs_Start;
    // unsigned long n = gcd(3300, 24);
    printk(KERN_INFO "The greatest common devisor of 3300 and 24 is: %lu\n", gcd(3300, 24));
    printk(KERN_INFO "Number of timer interupts[end]: %lu\n", jiffies);
    printk(KERN_INFO "Time Elapsed: %lu \n", timeElapsed/HZ);
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");


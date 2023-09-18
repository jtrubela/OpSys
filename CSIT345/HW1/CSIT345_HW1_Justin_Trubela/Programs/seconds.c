/*
 seconds.c
 By Justin Trubela
 x
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 
 Using Java codebytes to complete task for assignment 7
 
 Purpose: Design a kernel module that creates a proc file named /proc/seconds
 that reports the number of elapsed seconds since the kernel module was loaded.
 This will involve using the value of jiffies as well as the HZ rate.
 When a user enters the command -"cat /proc/seconds"-
 your kernel module will report the number of seconds that have elapsed
 since the kernel module was first loaded.
 Be sure to remove /proc/seconds when the module is removed.
 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/param.h>        //Import the c parameters for timing/hz and jiffies library
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds" //Make the Process Name the required "seconds"
#define MESSAGE "Number of seconds elapsed since Module Start: "
/**
 * Function prototypes
 */
int INITIAL, FINAL;         //Declare the seconds variables to make the time calculation
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};


/* This function is called when the module is loaded. */
int proc_init(void)
{
    
    // creates the /proc/seconds entry
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    INITIAL = jiffies/HZ;       //Make the necessary calculation to get the intial timing
    return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {
    
    // removes the /proc/hello entry
    remove_proc_entry(PROC_NAME, NULL);
    
    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/seconds is read.
 *
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    
    if (completed) {
        completed = 0;
        return 0;
    }
    
    completed = 1;
    FINAL = jiffies /HZ;                      //Make the necessary calculations to get the Final time
    rv = sprintf(buffer, "%s %d\n",MESSAGE, FINAL - INITIAL);  //Output the timing
    
    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);
    
    return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");

/*
 jiffies.c
 By Justin Trubela
 
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 
 Using Java codebytes to complete task for assignment 7
 
 Design a kernel module that creates a /proc file named /proc/jiffies
 that reports the current value of jiffies when the /proc/jiffies file
 is read, such as with the command -"cat /proc/jiffies"-
 Be sure to remove /proc/jiffies when the module is removed.
 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
// #include <linux/fs.h>
// #include <linux/device.h>
#include <linux/jiffies.h>      //Import the c parameters for timing/hz and jiffies library

#define BUFFER_SIZE 128
#define PROC_NAME "jiffies"     //Define Process Name jiffies
#define MESSAGE "number of jiffies is"
/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

/* This function is called when the module is loaded. */
int proc_init(void)
{
    
    // creates the /proc/hello entry
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    
    return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {
    
    // removes the /proc/hello entry
    remove_proc_entry(PROC_NAME, NULL);
    
    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
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
    rv = sprintf(buffer, "Current number of jiffies is: %lu\n", jiffies);      //Output the value of jiffies
                                                                               // rv = sprintf(buffer, "JIFFFIES\n");      //Output the value of jiffies
    
    
    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);
    
    return rv;
}


// Macros for registering module entry and exit points
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");

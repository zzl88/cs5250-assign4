#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUMBER 65
#define DEV_SIZE 4 * 1024 * 1024

/* forward declaration */
int bytes_open(struct inode *inode, struct file *filep);
int bytes_release(struct inode *inode, struct file *filep);
ssize_t bytes_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t bytes_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
static void bytes_exit(void);

/* definition of file_operation structure */
struct file_operations bytes_fops = {
  read: bytes_read,
  write: bytes_write,
  open: bytes_open,
  release: bytes_release
};

char *bytes_data = NULL;
size_t stored = 0;

int bytes_open(struct inode *inode, struct file *filep)
{
  return 0; // always successful
}

int bytes_release(struct inode *inode, struct file *filep)
{
  return 0; // always successful
}

ssize_t bytes_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
  if (*f_pos > stored) return -1;
  if (count + (*f_pos) > stored) count = stored - *f_pos;
  copy_to_user(buf, bytes_data + *f_pos, count);
  *f_pos += count;
  return count;
}

ssize_t bytes_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
  if (*f_pos >= DEV_SIZE) return -ENOSPC;
  if (count + (*f_pos) > DEV_SIZE) count = DEV_SIZE - *f_pos;
  copy_from_user(bytes_data + *f_pos, buf, count);
  *f_pos += count;
  stored = *f_pos;
  if (stored == DEV_SIZE) return -ENOSPC;
  return count;
  /*
  if (count > 1) {
    printk(KERN_INFO "bytes: try to write %zu bytes %s\n", count, buf);
    return -ENOSPC;
  }
  */
}

static int bytes_init(void)
{
  int result;
  // register the device
  result = register_chrdev(MAJOR_NUMBER, "bytes", &bytes_fops);
  if (result < 0) {
    return result;
  }
  // allocate one byte of memory for storage
  // kmalloc is just like malloc, the second parameter is
  // the type of memory to be allocated.
  // To release the memory allocated by kmalloc, use kfree.
  bytes_data = kmalloc(DEV_SIZE, GFP_KERNEL);
  if (!bytes_data) {
    bytes_exit();
    // cannot allocate memory
    // return no memory error, negative signify a failure
    return -ENOMEM;
  }
  // initialize the value to be X
  *bytes_data = '\0';
  printk(KERN_ALERT "This is a bytes device module\n");
  return 0;
}

static void bytes_exit(void)
{
  // if the pointer is pointing to something
  if (bytes_data) {
    // free the memory and assign the pointer to NULL
    kfree(bytes_data);
    bytes_data = NULL;
  }

  // unregister the device
  unregister_chrdev(MAJOR_NUMBER, "bytes");
  printk(KERN_ALERT "bytes device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(bytes_init);
module_exit(bytes_exit);


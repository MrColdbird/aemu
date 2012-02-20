#ifndef _LOGS_H_
#define _LOGS_H_

// Check CPU Interrupt State
int is_cpu_intr_enable(void);

// Dump Binary Buffer to Logfile
void hexdump(void * addr, int size);

// Logfile API
#ifdef DEBUG
int printk_init(const char* filename);
int printk(char *fmt, ...)__attribute__((format (printf, 1, 2)));
int printk_sync(void);
void printk_lock(void);
void printk_unlock(void);
#else
#define printk_init(...)
#define printk(...)
#define printk_sync()
#define printk_lock()
#define printk_unlock()
#endif

// Unimplemented Stub Log
#define THROW_UNIMPLEMENTED(func) printk("UNIMPLEMENTED: %s\n", func)

#endif

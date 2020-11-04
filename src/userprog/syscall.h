#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "threads/thread.h"

typedef int pid_t;

void syscall_init(void);

/* project 1 */
void halt(void);
void exit(int status);
pid_t exec(const char *file);
int wait(pid_t pid);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);

/* project 2 */
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);

/* additional system calls */
int fibonacci(int n);
int max_of_four_int(int a, int b, int c, int d);

/* check invalid memory */
void is_valid_vaddr(const void *vaddr);

#endif /* userprog/syscall.h */

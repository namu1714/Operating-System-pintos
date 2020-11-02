#include "userprog/syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include "lib/user/syscall.h"

#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "devices/shutdown.h"
#include "devices/input.h"

static void syscall_handler (struct intr_frame *);

/* project 1 */
void halt(void);
void exit(int status);
pid_t exec(const char *file);
int wait(pid_t pid);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);

/* additional system calls */
int fibonacci(int n);
int max_of_four_int(int a, int b, int c, int d);

/* check invalid memory */
void is_valid_vaddr(const void *vaddr);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  //printf("system call: %d\n", *(int*)(f->esp));
  //hex_dump((uintptr_t)(f->esp), f->esp, 100, true);
  
  is_valid_vaddr(f->esp);

  switch(*(uint32_t *)(f->esp)){
  case SYS_HALT:
    halt();
    break;
  case SYS_EXIT:
    is_valid_vaddr(f->esp + 4);
    exit(*(int*)(f->esp + 4));
    break;
  case SYS_EXEC:
    is_valid_vaddr(f->esp + 4);
    is_valid_vaddr((const void *)*(uint32_t*)(f->esp + 4));
    f->eax = exec((const char *)*(uint32_t*)(f->esp + 4));
    break;
  case SYS_WAIT:
    is_valid_vaddr(f->esp + 4);
    f->eax = wait(*(int*)(f->esp + 4));
    break;
  case SYS_WRITE:
    is_valid_vaddr(f->esp + 4);
    is_valid_vaddr(f->esp + 8);
    is_valid_vaddr(f->esp + 12);
    f->eax = write(*(int*)(f->esp + 4), (void*)*(uint32_t*)(f->esp + 8), *(uint32_t*)(f->esp + 12));
    break;
  case SYS_READ:
    is_valid_vaddr(f->esp + 4);
    is_valid_vaddr(f->esp + 8);
    is_valid_vaddr(f->esp + 12);
    f->eax = read(*(int*)(f->esp + 4), (void*)*(uint32_t*)(f->esp + 8), *(uint32_t*)(f->esp + 12));
    break;
  case SYS_FIBONACCI:
    is_valid_vaddr(f->esp + 4);
    f->eax = fibonacci(*(int*)(f->esp + 4));
    break;
  case SYS_MAX_OF_FOUR_INT:
    is_valid_vaddr(f->esp + 4);
    is_valid_vaddr(f->esp + 8);
    is_valid_vaddr(f->esp + 12);
    is_valid_vaddr(f->esp + 16);
    f->eax = max_of_four_int(*(int*)(f->esp + 4), *(int*)(f->esp + 8), *(int*)(f->esp + 12), *(int*)(f->esp + 16));
    break;
  }
  //printf ("system call!\n");
  //thread_exit();
}

/* Terminate Pintos. */
void halt(void){
  shutdown_power_off();
}

/* Terminate the current user program. */
void exit(int status){
  struct thread *cur = thread_current();
  printf("%s: exit(%d)\n", cur->name, status);
  cur->exit_status = status;
  thread_exit();
}

/* Create child process. */
pid_t exec(const char *file){
  pid_t id = process_execute(file); 
  return id;
}

/* Wait child process until it finishes its work. */
int wait(pid_t pid){
  int status = process_wait(pid);
  //printf("wait status:%d\n", status);
  return status;
}

/* Reads size bytes from the file open as fd into buffer.
 * Returns the number of bytes read or -1 if the file could not be read.
 * Fd 0 reads from the keyboard using input_getc().*/
int read (int fd, void *buffer, unsigned size)
{
  uint8_t key;
  int i, ret = -1;

  if (fd == 0) { //standard input
    for(i=0; i<(int)size; i++){
      key = input_getc();
      *(uint8_t*)(buffer + i) = key;
    }
    ret = size;
  }

  return ret;
}

/* Write size bytes from buffer to the open file fd. 
 * Returns the number of butes actually written.
 * Fd 1 writes to the console.*/ 
int write (int fd, const void *buffer, unsigned size)
{
  int ret = -1;
  if (fd == 1) { //standard output
    putbuf(buffer, size);
    ret = size;
  }

  return ret;
}

/* Get nth fibonacci sequence number. */
int fibonacci(int n){
  int first, second, third;
  if(n < 0){ //if n is negative number
    return -1;
  }
  if(n == 0){
    return 0;
  }
  else if(n == 1){
    return 1;
  }
  
  first = 0; second = 1;
  for(int i=2; i<=n; i++){
    third = first + second;
    first = second;
    second = third;
  }
  return third;
}

/* Get max value among four integers. */
int max_of_four_int(int a, int b, int c, int d){
  int max = a;
  if(b>max) max = b;
  if(c>max) max = c;
  if(d>max) max = d;
  
  return max;
}

/* Check validity of user-provided pointer. 
 * Check if it uses kernel address space and if it is unmapped virtual memory. */
void is_valid_vaddr (const void *vaddr){
  if(!is_user_vaddr(vaddr) || pagedir_get_page(thread_current()->pagedir, vaddr) == NULL){
    exit(-1);
  }
}

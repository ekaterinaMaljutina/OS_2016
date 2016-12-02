#ifndef __THREADS_H
#define __THREADS_H

#include <stdint.h>

#define MAX_THREADS (1 << 12)
#define THREAD_STACK_SIZE 0x1000

#define RUNNING 1
#define JOIN 0
	
typedef struct thread {
  void* stack_mem_begin;
  void* stack_pointer;
  
  int state;
  struct thread *prev;
  struct thread *next;
  
  uint32_t lock_count;
} thread_t;

typedef void (*func)(void*);

thread_t *thread_current;
thread_t thread_kernel;

void init_threads();
void thread_schedule();
void thread_switch(thread_t *other);

thread_t* thread_current_();
void thread_lock();
void thread_unlock();
thread_t* thread_create(func func_, void *arg);
void thread_join(thread_t *other);

#endif /* __THREADS_H */
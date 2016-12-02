#include "threads.h"

#include "lock.h"
#include "print.h"
#include "memory.h"
#include "alloc.h"
#include "ints.h"


extern void switch_threads(void **prev, void *next);
extern void thread_origin_(void);


void init_threads() {
  thread_current = &thread_kernel;
  
  thread_current->stack_mem_begin = NULL;
  thread_current->stack_pointer = NULL;
  
  thread_current->state = RUNNING;

  thread_current->prev = thread_current;
  thread_current->next = thread_current;
  
  thread_current->lock_count = 0;

  printf("threads init ... ok\n");
}

void thread_switch(thread_t *other) {
  if (other == thread_current) {
    return;
  }

  thread_t *previous = thread_current;
  thread_current = other;
  
  switch_threads(&previous->stack_pointer, other->stack_pointer);
}

void thread_schedule() {
  thread_switch(thread_current->next);
}

volatile int critical_section_level = 0;

void thread_lock(){
    disable_ints();
    __sync_fetch_and_add(&critical_section_level, 1);
}
void thread_unlock(){
  if (__sync_fetch_and_add(&critical_section_level, -1) == 1) 
        enable_ints();
}

void thread_origin(func func_, void *arg) {
  func_(arg);
  thread_lock();
  thread_current->state = JOIN;
  thread_unlock();
  while(1);
}

thread_t* thread_create(func func_, void *arg){
  thread_lock();

  thread_t* new_thread = (thread_t*) mem_alloc(sizeof(thread_t));

  new_thread->stack_mem_begin = mem_alloc(THREAD_STACK_SIZE);
  new_thread->stack_pointer = (void*)( (uint64_t)new_thread->stack_mem_begin + THREAD_STACK_SIZE);


  new_thread->prev = thread_current;
  new_thread->next = thread_current->next;
  new_thread->prev->next = new_thread;
  new_thread->next->prev = new_thread;

  struct init_thread_data  {
      uint64_t rflags, r15, r14, r13, r12, rbx, rbp;
      void* start_thread_address;
  };

  new_thread->stack_pointer = (uint8_t *)new_thread->stack_pointer - sizeof(struct init_thread_data);

  struct init_thread_data* init_value = new_thread->stack_pointer;
  init_value->start_thread_address = (void*) (uint64_t) thread_origin_;;
  init_value->rflags = 0;
  init_value->r12 = 0;
  init_value->r13 = 0;
  init_value->r14 = (uint64_t) arg;
  init_value->r15 = (uint64_t) func_;
  init_value->rbx = 0;
  init_value->rbp = 0;

  new_thread->state = RUNNING;
  thread_unlock();
  return new_thread;
}

void thread_join(thread_t *other) {
  printf("thread_join begin\n");
  while (1) {
    thread_lock();
    if (other->state != JOIN) {
      thread_unlock();
      continue;
    }
    
    break;
  }

  other->prev->next = other->next;
  other->next->prev = other->prev;
  mem_free(other->stack_pointer);
  mem_free(other);
  thread_unlock();
}


thread_t* thread_current_(){
  return thread_current;
}
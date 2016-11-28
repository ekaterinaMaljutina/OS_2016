#ifndef __LOCK_H__
#define __LOCK_H__

#include <stdint.h>
#include <stdatomic.h>

#define LOCKED   1
#define UNLOCKED 0

typedef struct spinlock {
  atomic_int locked;
} spinlock_;

void lock(struct spinlock* lock);
void unlock(struct spinlock *lock);


#endif /* __LOCK_H__ */
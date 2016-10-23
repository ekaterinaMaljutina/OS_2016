#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};
typedef struct list_head list_;

#define CONTAINER_OF(ptr, type, member) \
    (type *)( (char *)(ptr) - offsetof(type, member) )
#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) list_ name = LIST_HEAD_INIT(name)
#define LIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)

void list_init(list_ *head);
void list_add(list_ *new, list_ *head);
void list_add_tail(list_ *new, list_ *head);
void list_del(list_ *entry);
void list_splice(list_ *list, list_ *head);
bool list_empty(const list_ *head);
list_ *list_first(list_ *head);
size_t list_size(const list_ *head);

#endif /*__LIST_H__*/

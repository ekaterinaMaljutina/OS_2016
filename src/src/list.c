#include "list.h"

void list_init(list_ *head){
    head->next = head->prev = head;
}

static void list_insert(list_ *new, list_ *prev,
            list_ *next){
    new->prev = prev;
    new->next = next;
    prev->next = new;
    next->prev = new;
}

void list_add(list_ *new, list_ *head){
    list_insert(new, head, head->next);
}

void list_add_tail(list_ *new, list_ *head){
    list_insert(new, head->prev, head);
}

static void __list_del(list_ *prev, list_ *next){
    prev->next = next;
    next->prev = prev;
}

void list_del(list_ *entry){
    __list_del(entry->prev, entry->next);
}

static void __list_splice(list_ *list, list_ *prev,
            list_ *next){
    list_ *first = list->next;
    list_ *last = list->prev;

    first->prev = prev;
    prev->next = first;
    last->next = next;
    next->prev = last;
}

void list_splice(list_ *list, list_ *head) {
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        list_init(list);
    }
}

bool list_empty(const list_ *head){
    return head->next == head;
}

list_ *list_first(list_ *head){
    return head->next;
}

size_t list_size(const list_ *head){
    size_t size = 0;

    for (const list_ *pos = head->next; pos != head;
        pos = pos->next)
        ++size;
    return size;
}

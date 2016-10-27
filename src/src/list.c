#include "list.h"



void create_node_list(node *ptr, u8 or, bool use, node *prev, node *next) {
    ptr->use = use;
    ptr->or  = or;
    ptr->prev = prev;
    ptr->next = next;
}

void push_front(node *n,  node** l) {
    if (l != NULL) {
        if (*l)
            (*l)->prev = n;                       
        n->next = (*l);
        (*l) = n;
    }                                  
}

void pop_front(node **list) {
    if (list && *list) {            
        *list = (*list)->next;      
        if (*list)              
            (*list)->prev = NULL;                       
    }                               
}

void pop_back(node *n) {
    if (n->prev) 
        n->prev->next = n->next;
    if (n->next)
        n->next->prev = n->prev;                                   
}

void create_node_list_and_push(node *ptr, u8 or, bool use, node *prev, node *next,node** l ){
    create_node_list(ptr,or,use,prev,next);
    push_front(ptr,l);
}
#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef struct d_linked_list node;

struct d_linked_list {
    node * prev;
    node * next;
    u8 use;
    u8 or;
};

void create_node_list( node *, u8, bool, node *, node * ) ;
void create_node_list_and_push(node *, u8, bool, node *, node *,node ** );
void push_front( node *,  node ** ) ;
void pop_front( node ** ) ;
void pop_back( node * ) ;


#endif /*__LIST_H__*/

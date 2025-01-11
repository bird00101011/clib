#include <types.h>
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node
{
    Object element;
    struct Node *prev;
    struct Node *next;
} LinkedListNode, *LPLinkedListNode;

typedef struct
{
    LPLinkedListNode lp_head;
    LPLinkedListNode lp_tail;
    long elements_num;
    long element_size;
} LinkedList, *LPLinkedList;

LPStatusDataException LinkedList_new(long element_size);
LPStatusDataException LinkedList_free(LPLinkedList lp_linkedlist);

LPStatusDataException LinkedList_insert(LPLinkedList lp_linkedlist, Object element, long position);

LPStatusDataException LinkedList_delete_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_delete_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_get_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_get_position_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_edit_by_position(LPLinkedList lp_linkedlist, long position, Object element);

LPStatusDataException LinkedList_edit_by_element(LPLinkedList lp_linkedlist, Object old_element, Object new_object);

#endif
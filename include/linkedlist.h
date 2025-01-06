#include <types.h>
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node
{
    Object element;
    struct Node *next;
    long elements_num;
} LinkedList, *LPLinkedList;

LPStatusDataException LinkedList_new();
LPStatusDataException LinkedList_free(LPLinkedList lp_linkedlist);

LPStatusDataException LinkedList_insert(LPLinkedList lp_linkedlist, Object element, long position);

LPStatusDataException LinkedList_delete_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_delete_element_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_get_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_get_position_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_edit_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_edit_element_by_element(LPLinkedList lp_linkedlist, Object element);

#endif
#include <types.h>
#include <dynaarray.h>
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node
{
    void *ele;
    struct Node *prev;
    struct Node *next;
} LinkedListNode, *LPLinkedListNode;

typedef struct
{
    LPLinkedListNode lp_head;
    LPLinkedListNode lp_tail;
    long eles_num;
    long ele_size;
    int (*copy_func)(void *, void *);
    int (*compare_func)(void *, void *);
    int (*free_func)(void *);
} LinkedList, *LPLinkedList;

int LinkedList_new(LPLinkedList lp_ll,
                   long ele_size,
                   int (*copy_func)(void *, void *),
                   int (*compare_func)(void *, void *),
                   int (*free_func)(void *));
int LinkedList_free(LPLinkedList lp_ll);

int LinkedList_insert(LPLinkedList lp_ll, void *ele, long pos);

int LinkedList_del_by_pos(LPLinkedList lp_ll, long pos);

int LinkedList_del_by_ele(LPLinkedList lp_ll, void *ele, LPDynaArray lp_poses);

int LinkedList_get_by_pos(LPLinkedList lp_ll, long pos, void *ele);

int LinkedList_get_pos_by_ele(LPLinkedList lp_ll, void *ele, LPDynaArray lp_poses);

int LinkedList_edit_by_pos(LPLinkedList lp_ll, long pos, void *ele);

int LinkedList_edit_by_ele(LPLinkedList lp_ll, void *old_ele, void *new_ele, LPDynaArray lp_poses);

#endif
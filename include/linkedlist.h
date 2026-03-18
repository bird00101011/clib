#include <dynaarray.h>
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node {
  void* ele;
  long ele_size;
  struct Node* prev;
  struct Node* next;
} LinkedListNode, *LPLinkedListNode;

typedef struct {
  LPLinkedListNode lp_head;
  LPLinkedListNode lp_tail;
  long eles_num;
  int (*copy_func)(void*, void*);
  int (*compare_func)(void*, void*);
  int (*free_func)(void*);
} LinkedList, *LPLinkedList;

__declspec(dllexport) int LinkedList_init(LPLinkedList lp_ll,
                                          int (*copy_func)(void*, void*),
                                          int (*compare_func)(void*, void*),
                                          int (*free_func)(void*));

__declspec(dllexport) int LinkedList_free(LPLinkedList lp_ll);

__declspec(dllexport) int LinkedList_insert(LPLinkedList lp_ll, long pos,
                                            void* ele, long ele_size);

__declspec(dllexport) int LinkedList_del_by_pos(LPLinkedList lp_ll, long pos);

__declspec(dllexport) int LinkedList_del_by_ele(LPLinkedList lp_ll, void* ele,
                                                long ele_size,
                                                LPDynaArray lp_poses);

__declspec(dllexport) int LinkedList_get_by_pos(LPLinkedList lp_ll, long pos,
                                                void* ele, long* ele_size);

__declspec(dllexport) void* LinkedList_get_addr_by_pos(LPLinkedList lp_ll,
                                                       long pos);

__declspec(dllexport) int LinkedList_get_pos_by_ele(LPLinkedList lp_ll,
                                                    void* ele, long ele_size,
                                                    LPDynaArray lp_poses);

__declspec(dllexport) int LinkedList_edit_by_pos(LPLinkedList lp_ll, long pos,
                                                 void* ele, long ele_size);

__declspec(dllexport) int LinkedList_edit_by_ele(
    LPLinkedList lp_ll, void* old_ele, long old_ele_size, void* new_ele,
    long new_ele_size, LPDynaArray lp_poses);

#endif
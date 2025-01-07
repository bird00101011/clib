#include <types.h>
#include <linkedlist.h>
#include <stdlib.h>

LPStatusDataException LinkedList_new()
{
    LPStatusDataException lpsde = StatusDataException_new();
    if (lpsde == NULL_POINTER)
        return lpsde;

    LPLinkedList lp_linkedlist = malloc(sizeof(LinkedList));
    if (lp_linkedlist == NULL_POINTER)
    {
        lpsde->lp_exception->error_null_pointer = True;
        lpsde->status = False;
        return lpsde;
    }

    lp_linkedlist->lp_head = NULL_POINTER;
    lp_linkedlist->elements_num = 0;
    lpsde->data = lp_linkedlist;
    return lpsde;
}

LPStatusDataException LinkedList_free(LPLinkedList lp_linkedlist)
{
    LPStatusDataException lpsde = StatusDataException_new();
    if (lpsde == NULL_POINTER)
        return lpsde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lpsde->lp_exception->error_null_pointer = True;
        lpsde->status = False;
        return lpsde;
    }

    LPLinkedListNode lp_next = lp_linkedlist->lp_head;
    while (lp_next != NULL_POINTER)
    {
        free(lp_next);
        LPLinkedListNode lp_next = lp_linkedlist->lp_head;
    }

    free(lp_linkedlist);
    return lpsde;
}

LPStatusDataException LinkedList_insert(LPLinkedList lp_linkedlist, Object element, long position)
{
    LPStatusDataException lpsde = StatusDataException_new();
    if (lpsde == NULL_POINTER)
        return lpsde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lpsde->lp_exception->error_null_pointer = True;
        lpsde->status = False;
        return lpsde;
    }
    if (position < 0 && position > lp_linkedlist->elements_num - 1)
    {
        lpsde->lp_exception->error_index_out = True;
        lpsde->status = False;
        return lpsde;
    }

    LPLinkedListNode lp_next = lp_linkedlist->lp_head;
    for (long i = 1; i <= position; i++)
    {
        lp_next = lp_next->next;
    }

    lpsde->data = lp_linkedlist;

    LPLinkedListNode lp_new = malloc(sizeof(LinkedListNode));
    if (lp_new == NULL_POINTER)
    {
        lpsde->status = False;
        lpsde->lp_exception->error_malloc = True;
        return lpsde;
    }

    lp_new->element = element;
    lp_new->next = lp_next->next;
    lp_next->next = lp_new;

    lp_linkedlist->elements_num++;

    lpsde->data = lp_linkedlist;
    return lpsde;
}

LPStatusDataException LinkedList_delete_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_delete_element_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_get_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_get_position_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_edit_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_edit_element_by_element(LPLinkedList lp_linkedlist, Object element);
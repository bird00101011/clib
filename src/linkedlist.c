#include <types.h>
#include <linkedlist.h>
#include <stdlib.h>

LPStatusDataException LinkedList_new(long element_size)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    LPLinkedList lp_linkedlist = malloc(sizeof(LinkedList));
    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_linkedlist->lp_head = NULL_POINTER;
    lp_linkedlist->lp_tail = NULL_POINTER;
    lp_linkedlist->elements_num = 0;
    lp_linkedlist->element_size = element_size;
    lp_sde->data = lp_linkedlist;
    return lp_sde;
}

LPStatusDataException LinkedList_free(LPLinkedList lp_linkedlist)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    LPLinkedListNode lp_next = lp_linkedlist->lp_head;
    while (lp_next != NULL_POINTER)
    {
        free(lp_next);
        LPLinkedListNode lp_next = lp_linkedlist->lp_head;
    }

    free(lp_linkedlist);
    return lp_sde;
}

LPStatusDataException LinkedList_insert(LPLinkedList lp_linkedlist, Object element, long position)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    if (position < 0 && position > lp_linkedlist->elements_num)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_sde->data = lp_linkedlist;

    LPLinkedListNode lp_new = malloc(sizeof(LinkedListNode));
    if (lp_new == NULL_POINTER)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_malloc = True;
        return lp_sde;
    }

    int ls = sizeof(long);
    lp_new->element = malloc(ls);
    if (memcpy((char *)lp_new->element, (char *)element, lp_linkedlist->element_size) == NULL_POINTER)
    {
        free(lp_new->element);
        free(lp_new);

        lp_sde->lp_exception->error_memcpy = True;
        lp_sde->status = False;
        return lp_sde;
    }

    LPLinkedListNode lp_head_tmp = lp_linkedlist->lp_head;
    LPLinkedListNode lp_tail_tmp = lp_linkedlist->lp_tail;
    if (position == 0)
    {
        lp_linkedlist->lp_head = lp_new;
        lp_new->prev = NULL_POINTER;

        if (lp_linkedlist->elements_num == 0)
        {
            lp_new->next = NULL_POINTER;
            lp_linkedlist->lp_tail = lp_new;
        }
        else
        {
            lp_new->next = lp_head_tmp;
            lp_head_tmp->prev = lp_new;
        }
    }
    else
    {
        if (position == lp_linkedlist->elements_num)
        {
            lp_tail_tmp->next = lp_new;
            lp_new->prev = lp_tail_tmp;
            lp_new->next = NULL_POINTER;
            lp_linkedlist->lp_tail = lp_new;
        }
        else
        {
            LPLinkedListNode lp_next;
            if (position <= abs(lp_linkedlist->elements_num - position))
            {
                lp_next = lp_head_tmp;
                for (long i = 1; i <= position; i++)
                    lp_next = lp_next->next;
            }
            else
            {
                lp_next = lp_tail_tmp;
                for (long i = lp_linkedlist->elements_num - 2; i >= position; i--)
                    lp_next = lp_next->prev;
            }

            lp_new->next = lp_next;
            lp_new->prev = lp_next->prev;
            lp_next->prev = lp_new;
        }
    }

    lp_linkedlist->elements_num++;

    lp_sde->data = lp_linkedlist;
    return lp_sde;
}

LPStatusDataException LinkedList_delete_by_position(LPLinkedList lp_linkedlist, long position)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    lp_sde->data = lp_linkedlist;
    if (position < 0 && position > lp_linkedlist->elements_num - 1)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
    }
    else
    {
        LPLinkedListNode lp_node = lp_linkedlist->lp_head;
        for (long i = 1; i <= position; i++)
        {
            lp_node = lp_node->next;
        }
        lp_node->prev->next = lp_node->next;
        lp_node->next->prev = lp_node->prev;
        free(lp_node);
    }
    return lp_sde;
}

LPStatusDataException LinkedList_delete_by_element(LPLinkedList lp_linkedlist, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    LPLinkedListNode lp_next = lp_linkedlist->lp_head;
    long ds = 0;
    int ls = sizeof(long);
    for (long i = 1; i < lp_linkedlist->elements_num; i++)
    {
        if (memcmp((char *)lp_next, (char *)element, lp_linkedlist->element_size) == 0)
        {
            lp_node->prev->next = lp_node->next;
            lp_node->next->prev = lp_node->prev;
            free(lp_node);
            ds++;
        }
    }
    lp_sde->data = malloc(ls);
    if (memcpy((char *)lp_sde->data, (char *)&ds, ls) == NULL_POINTER)
    {
        lp_sde->lp_exception->error_memcpy = True;
        lp_sde->status = True;
    }
    return lp_sde;
}

LPStatusDataException LinkedList_get_by_position(LPLinkedList lp_linkedlist, long position)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    LPLinkedListNode lp_next = lp_linkedlist->lp_head;
}

LPStatusDataException LinkedList_get_position_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_edit_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_edit_element_by_element(LPLinkedList lp_linkedlist, Object element);
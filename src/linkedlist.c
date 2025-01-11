#include <types.h>
#include <linkedlist.h>
#include <dynaarray.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

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
    LPLinkedListNode lp_tmp = lp_next->next;
    free(lp_next->element);
    free(lp_next);
    lp_next = lp_tmp;
    for (long i = 1; i < lp_linkedlist->elements_num - 1; i++)
    {
        lp_tmp = lp_next->next;
        free(lp_next->element);
        free(lp_next);
        lp_next = lp_tmp;
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
    if (position < 0 || position > lp_linkedlist->elements_num)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
        return lp_sde;
    }

    LPLinkedListNode lp_new = malloc(sizeof(LinkedListNode));
    if (lp_new == NULL_POINTER)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_malloc = True;
        return lp_sde;
    }

    lp_new->element = malloc(lp_linkedlist->element_size);
    if (lp_new->element == NULL_POINTER)
    {
        free(lp_new);
        lp_sde->lp_exception->error_malloc = True;
        lp_sde->status = False;
        return lp_sde;
    }
    if (memcpy((char *)lp_new->element, (char *)element, lp_linkedlist->element_size) == NULL_POINTER)
    {
        free(lp_new->element);
        free(lp_new);
        lp_sde->lp_exception->error_memcpy = True;
        lp_sde->status = False;
        return lp_sde;
    }
    lp_new->prev = NULL_POINTER;
    lp_new->next = NULL_POINTER;

    LPLinkedListNode lp_head_tmp = lp_linkedlist->lp_head;
    LPLinkedListNode lp_tail_tmp = lp_linkedlist->lp_tail;
    if (lp_linkedlist->elements_num == 0)
    {
        lp_linkedlist->lp_head = lp_new;
        lp_linkedlist->lp_tail = lp_new;
    }
    else if (lp_linkedlist->elements_num == 1)
    {
        if (position == 0)
        {
            lp_linkedlist->lp_head = lp_new;
            lp_new->next = lp_head_tmp;
            lp_head_tmp->prev = lp_new;
            lp_linkedlist->lp_tail = lp_head_tmp;
        }
        else
        {
            lp_linkedlist->lp_tail = lp_new;
            lp_new->prev = lp_head_tmp;
            lp_head_tmp->next = lp_new;
        }
    }
    else
    {
        if (position == 0)
        {
            lp_linkedlist->lp_head = lp_new;
            lp_linkedlist->lp_head->next = lp_head_tmp;
            lp_head_tmp->prev = lp_new;
        }
        else if (position == lp_linkedlist->elements_num)
        {
            lp_linkedlist->lp_tail = lp_new;
            lp_new->prev = lp_tail_tmp;
            lp_tail_tmp->next = lp_new;
        }
        else
        {
            LPLinkedListNode lp_next;
            if (position < abs(lp_linkedlist->elements_num - position))
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

            lp_next->prev->next = lp_new;
            lp_new->prev = lp_next->prev;
            lp_new->next = lp_next;
            lp_next->prev = lp_new;
        }
    }

    lp_linkedlist->elements_num++;

    lp_sde->data = lp_new->element;
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
    if (position < 0 || position > lp_linkedlist->elements_num - 1)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_index_out = True;
    }
    if (lp_linkedlist->elements_num == 1)
    {
        free(lp_linkedlist->lp_head->element);
        free(lp_linkedlist->lp_head);
        lp_linkedlist->lp_head = NULL_POINTER;
        lp_linkedlist->lp_tail = NULL_POINTER;
        lp_linkedlist->elements_num--;
    }
    else
    {
        LPLinkedListNode lp_head_tmp = lp_linkedlist->lp_head;
        LPLinkedListNode lp_tail_tmp = lp_linkedlist->lp_tail;
        LPLinkedListNode lp_next;

        if (position == 0)
        {
            lp_next = lp_head_tmp->next;
            lp_next->prev = NULL_POINTER;
            free(lp_head_tmp->element);
            free(lp_head_tmp);
            lp_linkedlist->lp_head = lp_next;
        }
        else if (position == lp_linkedlist->elements_num - 1)
        {
            lp_next = lp_tail_tmp->prev;
            lp_next->next = NULL_POINTER;
            free(lp_tail_tmp->element);
            free(lp_tail_tmp);
            lp_linkedlist->lp_tail = lp_next;
        }
        else
        {
            if (position < abs(position - lp_linkedlist->elements_num - 1))
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

            lp_next->prev->next = lp_next->next;
            lp_next->next->prev = lp_next->prev;
            free(lp_next->element);
            free(lp_next);
        }
        lp_linkedlist->elements_num--;
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
    int ls = sizeof(long);
    LPStatusDataException lp_sde_dn = DynaArray_new(10, ls);
    if (lp_sde_dn == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    if (lp_sde_dn->status == False)
    {
        StatusDataException_free(lp_sde);
        return lp_sde_dn;
    }
    LPDynaArray lp_da_pos = (LPDynaArray)lp_sde_dn->data;
    StatusDataException_free(lp_sde_dn);

    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
    }
    else if (lp_linkedlist->elements_num == 1)
    {
        if (memcmp((char *)lp_linkedlist->lp_head->element, (char *)element, lp_linkedlist->element_size) == 0)
        {
            free(lp_linkedlist->lp_head->element);
            free(lp_linkedlist->lp_head);
            lp_linkedlist->lp_head = NULL_POINTER;
            lp_linkedlist->lp_tail = NULL_POINTER;
            lp_linkedlist->elements_num--;
            long i = 0;
            lp_sde_dn = DynaArray_insert(lp_da_pos, i, &i);

            if (lp_sde_dn == NULL_POINTER || lp_sde_dn->status == False)
                lp_sde->lp_exception->error_some++;

            lp_linkedlist->elements_num = 0;
        }
        else
            lp_sde->status = False;
    }
    else
    {
        LPLinkedListNode lp_iter, lp_tmp;
        lp_iter = lp_linkedlist->lp_head;
        if (memcmp((char *)lp_iter, (char *)element, lp_linkedlist->element_size) == 0)
        {
            lp_tmp = lp_iter->next;
            free(lp_iter->element);
            free(lp_iter);
            lp_iter = lp_tmp;
            lp_linkedlist->lp_head = lp_iter;
            lp_iter->prev = NULL_POINTER;
            lp_linkedlist->elements_num--;
        }
        long z = lp_linkedlist->elements_num;
        for (long j = 1; j < z; j++)
        {
            if (memcmp((char *)lp_iter, (char *)element, lp_linkedlist->element_size) == 0)
            {
                lp_tmp = lp_iter->next;
                lp_tmp->prev = lp_iter->prev;
                lp_iter->prev->next = lp_tmp;

                if (lp_tmp->prev == NULL_POINTER)
                    lp_linkedlist->lp_head = lp_tmp;
                if (lp_tmp->next == NULL_POINTER)
                    lp_linkedlist->lp_tail = lp_tmp;

                free(lp_iter->element);
                free(lp_iter);
                lp_iter = lp_tmp;
                lp_linkedlist->elements_num--;
            }
            lp_iter = lp_iter->next;
        }
    }
    lp_sde->data = lp_da_pos;
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
    if (position < 0 || position > lp_linkedlist->elements_num)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
    }
    else if (lp_linkedlist->elements_num == 1)
    {
        lp_sde->data = lp_linkedlist->lp_head->element;
    }
    else
    {
        LPLinkedListNode lp_next;
        if (position < abs(position - lp_linkedlist->elements_num))
        {
            lp_next = lp_linkedlist->lp_head;
            for (long i = 1; i <= position; i++)
                lp_next = lp_next->next;
        }
        else
        {
            lp_next = lp_linkedlist->lp_tail;
            for (long i = lp_linkedlist->elements_num - 2; i >= position; i--)
                lp_next = lp_next->prev;
        }
        lp_sde->data = lp_next->element;
    }
    return lp_sde;
}

LPStatusDataException LinkedList_get_position_by_element(LPLinkedList lp_linkedlist, Object element);

LPStatusDataException LinkedList_edit_element_by_position(LPLinkedList lp_linkedlist, long position);

LPStatusDataException LinkedList_edit_element_by_element(LPLinkedList lp_linkedlist, Object element);
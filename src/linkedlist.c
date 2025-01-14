// c linked list implementation
#include <types.h>
#include <linkedlist.h>
#include <dynaarray.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

/**
 * Create a new linked list
 *
 * params:
 *  long element_size: the number of bytes of a linked list element
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_malloc]
 */
LPStatusDataException LinkedList_new(long element_size)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    LPLinkedList lp_linkedlist = malloc(sizeof(LinkedList));
    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
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

/**
 * Free up linked list memory
 *
 * params:
 *  LPLinkedList lp_linkedlist: liked list pointer
 *  Boolean (*func)(Object): deletes the function pointer for an element's sub-property
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_callback, error_index_out]
 */
LPStatusDataException LinkedList_free(LPLinkedList lp_linkedlist, Boolean (*func)(Object))
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }
    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    LPLinkedListNode lp_next = lp_linkedlist->lp_head;
    LPLinkedListNode lp_tmp = lp_next->next;
    if (False == func(lp_next->element))
    {
        lp_sde->lp_exception->error_callback++;
        lp_sde->status = False;
    }
    free(lp_next->element);
    free(lp_next);
    lp_next = lp_tmp;
    for (long i = 1; i < lp_linkedlist->elements_num - 1; i++)
    {
        lp_tmp = lp_next->next;
        if (func != NULL_POINTER)
        {
            if (False == func(lp_next->element))
            {
                lp_sde->lp_exception->error_callback++;
                lp_sde->status = False;
            }
        }
        free(lp_next->element);
        free(lp_next);
        lp_next = lp_tmp;
    }

    free(lp_linkedlist);
    return lp_sde;
}

/**
 * Inserts an element at a specified location in the linked list
 *
 * params:
 *  LPLinkedList lp_linkedlist: linked list pointer
 *  Object element: the element to be inserted
 *  long position: the position to be inserted
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_malloc, error_index_out, error_memcpy]
 */
LPStatusDataException LinkedList_insert(LPLinkedList lp_linkedlist, Object element, long position)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (position < 0 || position > lp_linkedlist->elements_num)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    LPLinkedListNode lp_new = malloc(sizeof(LinkedListNode));
    if (lp_new == NULL_POINTER)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_malloc++;
        return lp_sde;
    }

    lp_new->element = malloc(lp_linkedlist->element_size);
    if (lp_new->element == NULL_POINTER)
    {
        free(lp_new);
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (memcpy((char *)lp_new->element, (char *)element, lp_linkedlist->element_size) == NULL_POINTER)
    {
        free(lp_new->element);
        free(lp_new);
        lp_sde->lp_exception->error_memcpy++;
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

/**
 * Deletes elements based on their position
 *
 * params:
 *  LPLinkedList lp_linkedlist: linked list pointer
 *  long position: the position to delete
 *  Boolean (*func)(Object): the callback function to delte attribute of element memory
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_index_out, error_callback]
 */
LPStatusDataException LinkedList_delete_by_position(LPLinkedList lp_linkedlist, long position, Boolean (*func)(Object))
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }
    lp_sde->data = lp_linkedlist;
    if (position < 0 || position > lp_linkedlist->elements_num - 1)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_index_out++;
    }
    else if (lp_linkedlist->elements_num == 1)
    {
        if (func != NULL_POINTER)
        {
            if (func(lp_linkedlist->lp_head->element) == False)
            {
                lp_sde->status = False;
                lp_sde->lp_exception->error_callback++;
            }
        }
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
            if (func != NULL_POINTER)
            {
                if (func(lp_head_tmp->element) == False)
                {
                    lp_sde->status = False;
                    lp_sde->lp_exception->error_callback++;
                }
            }
            lp_next = lp_head_tmp->next;
            lp_next->prev = NULL_POINTER;
            free(lp_head_tmp->element);
            free(lp_head_tmp);
            lp_linkedlist->lp_head = lp_next;
        }
        else if (position == lp_linkedlist->elements_num - 1)
        {
            if (func != NULL_POINTER)
            {
                if (func(lp_tail_tmp->element) == False)
                {
                    lp_sde->status = False;
                    lp_sde->lp_exception->error_callback++;
                }
            }
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

            if (func != NULL_POINTER)
            {
                if (func(lp_next->element) == False)
                {
                    lp_sde->status = False;
                    lp_sde->lp_exception->error_callback++;
                }
            }
            free(lp_next->element);
            free(lp_next);
        }
        lp_linkedlist->elements_num--;
    }
    return lp_sde;
}

/**
 * Delete elements based on their contents
 *
 * params:
 *  LPLinkedList lp_linkedlist: linked list pointer
 *  Object element: the element to delete
 *  Boolean (*func)(Object): the callback function to delete attribute of element
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_malloc, error_index_out, error_memset, error_memove, error_memcpy]
 */
LPStatusDataException LinkedList_delete_by_element(LPLinkedList lp_linkedlist, Object element, Boolean (*func)(Object))
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }
    int ls = sizeof(long);
    LPStatusDataException lp_sde_dn = DynaArray_new(10, ls);
    if (lp_sde_dn == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }
    if (lp_sde_dn->status == False)
    {
        lp_sde->lp_exception->error_index_out += lp_sde_dn->lp_exception->error_index_out;
        lp_sde->lp_exception->error_malloc += lp_sde_dn->lp_exception->error_malloc;
        lp_sde->lp_exception->error_memset += lp_sde_dn->lp_exception->error_memset;
        lp_sde->status = False;
        StatusDataException_free(lp_sde_dn);
        return lp_sde;
    }
    LPDynaArray lp_da_pos = (LPDynaArray)lp_sde_dn->data;
    StatusDataException_free(lp_sde_dn);

    long i = 0;
    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
    }
    else if (lp_linkedlist->elements_num == 1)
    {
        if (memcmp((char *)lp_linkedlist->lp_head->element, (char *)element, lp_linkedlist->element_size) == 0)
        {
            if (func != NULL_POINTER)
            {
                if (False == func(lp_iter->element))
                {
                    lp_sde->status = False;
                    lp_sde->lp_exception->error_callback++;
                }
            }
            free(lp_linkedlist->lp_head->element);
            free(lp_linkedlist->lp_head);
            lp_linkedlist->lp_head = NULL_POINTER;
            lp_linkedlist->lp_tail = NULL_POINTER;
            lp_linkedlist->elements_num--;
            lp_sde_dn = DynaArray_insert(lp_da_pos, lp_da_pos->elements_num, &i);

            if (lp_sde_dn == NULL_POINTER)
            {
                lp_sde->lp_exception->error_malloc++;
                lp_sde->status = False;
            }
            else
            {
                if (lp_sde_dn->status == False)
                {
                    lp_sde->lp_exception->error_null_pointer += lp_sde_dn->lp_exception->error_null_pointer;
                    lp_sde->lp_exception->error_malloc += lp_sde_dn->lp_exception->error_malloc;
                    lp_sde->lp_exception->error_index_out += lp_sde_dn->lp_exception->error_index_out;
                    lp_sde->lp_exception->error_realloc += lp_sde_dn->lp_exception->error_realloc;
                    lp_sde->lp_exception->error_memset += lp_sde_dn->lp_exception->error_memset;
                    lp_sde->lp_exception->error_memove += lp_sde_dn->lp_exception->error_memove;
                    lp_sde->lp_exception->error_memcpy += lp_sde_dn->lp_exception->error_memcpy;
                    lp_sde->status = False;
                }
            }
            StatusDataException_free(lp_sde_dn);

            lp_linkedlist->elements_num = 0;
        }
        else
            lp_sde->status = False;
    }
    else
    {
        LPLinkedListNode lp_iter, lp_tmp;
        lp_iter = lp_linkedlist->lp_head;
        if (memcmp((char *)lp_iter->element, (char *)element, lp_linkedlist->element_size) == 0)
        {
            lp_tmp = lp_iter->next;
            if (func != NULL_POINTER)
            {
                if (False == func(lp_iter->element))
                {
                    lp_sde->status = False;
                    lp_sde->lp_exception->error_callback++;
                }
            }
            free(lp_iter->element);
            free(lp_iter);
            lp_iter = lp_tmp;
            lp_linkedlist->lp_head = lp_iter;
            lp_iter->prev = NULL_POINTER;
            lp_linkedlist->elements_num--;
            lp_sde_dn = DynaArray_insert(lp_da_pos, lp_da_pos->elements_num, &i);
            if (lp_sde_dn == NULL_POINTER)
            {
                lp_sde->lp_exception->error_malloc++;
                lp_sde->status = False;
            }
            if (lp_sde_dn->status == False)
            {
                lp_sde->lp_exception->error_null_pointer += lp_sde_dn->lp_exception->error_null_pointer;
                lp_sde->lp_exception->error_malloc += lp_sde_dn->lp_exception->error_malloc;
                lp_sde->lp_exception->error_index_out += lp_sde_dn->lp_exception->error_index_out;
                lp_sde->lp_exception->error_realloc += lp_sde_dn->lp_exception->error_realloc;
                lp_sde->lp_exception->error_memset += lp_sde_dn->lp_exception->error_memset;
                lp_sde->lp_exception->error_memove += lp_sde_dn->lp_exception->error_memove;
                lp_sde->lp_exception->error_memcpy += lp_sde_dn->lp_exception->error_memcpy;
                lp_sde->status = False;
            }
            StatusDataException_free(lp_sde_dn);
        }
        long z = lp_linkedlist->elements_num;
        for (i = 1; i < z; i++)
        {
            if (memcmp((char *)lp_iter->element, (char *)element, lp_linkedlist->element_size) == 0)
            {
                lp_tmp = lp_iter->next;
                lp_tmp->prev = lp_iter->prev;
                lp_iter->prev->next = lp_tmp;

                if (lp_tmp->prev == NULL_POINTER)
                    lp_linkedlist->lp_head = lp_tmp;
                if (lp_tmp->next == NULL_POINTER)
                    lp_linkedlist->lp_tail = lp_tmp;

                if (func != NULL_POINTER)
                {
                    if (False == func(lp_iter->element))
                    {
                        lp_sde->status = False;
                        lp_sde->lp_exception->error_callback++;
                    }
                }
                free(lp_iter->element);
                free(lp_iter);
                lp_iter = lp_tmp;
                lp_linkedlist->elements_num--;
                lp_sde_dn = DynaArray_insert(lp_da_pos, lp_da_pos->elements_num, &i);
                if (lp_sde_dn == NULL_POINTER)
                {
                    lp_sde->lp_exception->error_malloc++;
                    lp_sde->status = False;
                }
                if (lp_sde_dn->status == False)
                {
                    lp_sde->lp_exception->error_null_pointer += lp_sde_dn->lp_exception->error_null_pointer;
                    lp_sde->lp_exception->error_malloc += lp_sde_dn->lp_exception->error_malloc;
                    lp_sde->lp_exception->error_index_out += lp_sde_dn->lp_exception->error_index_out;
                    lp_sde->lp_exception->error_realloc += lp_sde_dn->lp_exception->error_realloc;
                    lp_sde->lp_exception->error_memset += lp_sde_dn->lp_exception->error_memset;
                    lp_sde->lp_exception->error_memove += lp_sde_dn->lp_exception->error_memove;
                    lp_sde->lp_exception->error_memcpy += lp_sde_dn->lp_exception->error_memcpy;
                    lp_sde->status = False;
                }
                StatusDataException_free(lp_sde_dn);
            }
            lp_iter = lp_iter->next;
        }
        if (memcmp((char *)lp_iter->element, (char *)element, lp_linkedlist->element_size) == 0)
        {
            lp_tmp = lp_iter->prev;
            lp_tmp->next = NULL_POINTER;
            lp_linkedlist->lp_tail = lp_tmp;
            lp_linkedlist->elements_num--;
            if (func != NULL_POINTER)
            {
                if (False == func(lp_iter->element))
                {
                    lp_sde->status = False;
                    lp_sde->lp_exception->error_callback++;
                }
            }
            free(lp_iter->element);
            free(lp_iter);
            lp_sde_dn = DynaArray_insert(lp_da_pos, lp_da_pos->elements_num, &i);
            if (lp_sde_dn == NULL_POINTER)
            {
                lp_sde->lp_exception->error_malloc++;
                lp_sde->status = False;
            }
            if (lp_sde_dn->status == False)
            {
                lp_sde->lp_exception->error_null_pointer += lp_sde_dn->lp_exception->error_null_pointer;
                lp_sde->lp_exception->error_malloc += lp_sde_dn->lp_exception->error_malloc;
                lp_sde->lp_exception->error_index_out += lp_sde_dn->lp_exception->error_index_out;
                lp_sde->lp_exception->error_realloc += lp_sde_dn->lp_exception->error_realloc;
                lp_sde->lp_exception->error_memset += lp_sde_dn->lp_exception->error_memset;
                lp_sde->lp_exception->error_memove += lp_sde_dn->lp_exception->error_memove;
                lp_sde->lp_exception->error_memcpy += lp_sde_dn->lp_exception->error_memcpy;
                lp_sde->status = False;
            }
            StatusDataException_free(lp_sde_dn);
        }
    }
    lp_sde->data = lp_da_pos;
    if (lp_da_pos->elements_num == 0)
        lp_sde->status = False;
    return lp_sde;
}

/**
 * Gets element based on its position
 *
 * params:
 *  LPLinkedList lp_linkedlist:
 *  long position:
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_index_out]
 */
LPStatusDataException LinkedList_get_by_position(LPLinkedList lp_linkedlist, long position)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }
    if (position < 0 || position > lp_linkedlist->elements_num)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
    }
    else if (lp_linkedlist->elements_num == 1)
        lp_sde->data = lp_linkedlist->lp_head->element;
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

/**
 * Gets an element index based on its content
 *
 * params:
 *  LPLinkedList lp_linkedlist: linked list pointer
 *  Object element: the element to search
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_malloc, error_index_out, error_realloc, error_memset,
 *              error_memove, error_memcpy]
 */
LPStatusDataException LinkedList_get_position_by_element(LPLinkedList lp_linkedlist, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER || element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    LPStatusDataException lp_sde_da = DynaArray_new(10, sizeof(long));
    if (lp_sde_da == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }
    if (lp_sde_da->status == False)
    {
        lp_sde->lp_exception->error_index_out += lp_sde_da->lp_exception->error_index_out;
        lp_sde->lp_exception->error_malloc += lp_sde_da->lp_exception->error_malloc;
        lp_sde->lp_exception->error_memset += lp_sde_da->lp_exception->error_memset;
        lp_sde->status = False;
        StatusDataException_free(lp_sde_da);
        return lp_sde;
    }

    LPDynaArray lp_da = (LPDynaArray)lp_sde_da->data;
    StatusDataException_free(lp_sde_da);
    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
    }
    else
    {
        LPLinkedListNode lp_iter = lp_linkedlist->lp_head;
        for (long i = 0; i < lp_linkedlist->elements_num; i++)
        {
            if (memcmp((char *)lp_iter->element, (char *)element, lp_linkedlist->element_size) == 0)
            {
                lp_sde_da = DynaArray_insert(lp_da, lp_da->elements_num, &i);
                if (lp_sde_da == NULL_POINTER)
                {
                    lp_sde->lp_exception->error_malloc++;
                    lp_sde->status = False;
                }
                else
                {
                    if (lp_sde_da->status == False)
                    {
                        lp_sde->lp_exception->error_null_pointer += lp_sde_da->lp_exception->error_null_pointer;
                        lp_sde->lp_exception->error_malloc += lp_sde_da->lp_exception->error_malloc;
                        lp_sde->lp_exception->error_index_out += lp_sde_da->lp_exception->error_index_out;
                        lp_sde->lp_exception->error_realloc += lp_sde_da->lp_exception->error_realloc;
                        lp_sde->lp_exception->error_memset += lp_sde_da->lp_exception->error_memset;
                        lp_sde->lp_exception->error_memove += lp_sde_da->lp_exception->error_memove;
                        lp_sde->lp_exception->error_memcpy += lp_sde_da->lp_exception->error_memcpy;
                        lp_sde->status = False;
                    }
                }
            }
            lp_iter = lp_iter->next;
        }
    }

    lp_sde->data = lp_da;
    if (lp_da->elements_num == 0)
        lp_sde->status = False;
    return lp_sde;
}

/**
 * Edit element based on the element index
 *
 * params:
 *  LPLinkedList lp_linkedlist:
 *  long position:
 *  Object element:
 *
 * return:
 *
 * exceptions: [error_memcpy, error_index_out, error_malloc, error_memset, error_null_pointer]
 */
LPStatusDataException LinkedList_edit_by_position(LPLinkedList lp_linkedlist, long position, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (position < 0 || position >= lp_linkedlist->elements_num)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    LPStatusDataException lp_sde_da = DynaArray_new(10, sizeof(long));
    if (lp_sde_da == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_sde_da->status == False)
    {
        lp_sde->lp_exception->error_index_out += lp_sde_da->lp_exception->error_index_out;
        lp_sde->lp_exception->error_malloc += lp_sde_da->lp_exception->error_malloc;
        lp_sde->lp_exception->error_memset += lp_sde_da->lp_exception->error_memset;
        lp_sde->status = False;
        StatusDataException_free(lp_sde_da);
        return lp_sde;
    }

    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
    }
    else
    {
        LPLinkedListNode lp_iter;
        if (position < abs(position - lp_linkedlist->elements_num - 1))
        {
            lp_iter = lp_linkedlist->lp_head;
            for (long i = 1; i <= position; i++)
                lp_iter = lp_iter->next;
        }
        else
        {
            lp_iter = lp_linkedlist->lp_tail;
            for (long i = lp_linkedlist->elements_num - 2; i >= position; i--)
                lp_iter = lp_iter->prev;
        }
        if (memcpy((char *)lp_iter->element, (char *)element, lp_linkedlist->element_size) == NULL_POINTER)
        {
            lp_sde->lp_exception->error_memcpy++;
            lp_sde->status = False;
        }
        else
            lp_sde->data = lp_iter->element;
    }

    return lp_sde;
}

/**
 * Edit elements based on their content
 *
 * params:
 *  LPLinkedList lp_linkedlist: linked list pointer
 *  Object old_element: the element to be edit
 *  Object new_element: the element to replace
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_index_out, error_malloc, error_memset, error_memcpy,
 *              error_memove, error_realloc]
 */
LPStatusDataException LinkedList_edit_by_element(LPLinkedList lp_linkedlist, Object old_element, Object new_element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_linkedlist == NULL_POINTER || old_element == NULL_POINTER || new_element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_linkedlist->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    LPStatusDataException lp_sde_da = DynaArray_new(10, sizeof(long));
    if (lp_sde_da == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }
    if (lp_sde_da->status = False)
    {
        lp_sde->lp_exception->error_index_out += lp_sde_da->lp_exception->error_index_out;
        lp_sde->lp_exception->error_malloc += lp_sde_da->lp_exception->error_malloc;
        lp_sde->lp_exception->error_memset += lp_sde_da->lp_exception->error_memset;
        lp_sde->status = False;
        StatusDataException_free(lp_sde_da);
        return lp_sde;
    }

    LPDynaArray lp_da = (LPDynaArray)lp_sde_da->data;
    StatusDataException_free(lp_sde_da);
    LPLinkedListNode lp_iter = lp_linkedlist->lp_head;
    for (long i = 0; i < lp_linkedlist->elements_num; i++)
    {
        if (memcmp((char *)lp_iter->element, (char *)old_element, lp_linkedlist->element_size) == 0)
        {
            if (memcpy((char *)lp_iter->element, (char *)new_element, lp_linkedlist->element_size) == NULL_POINTER)
            {
                lp_sde->lp_exception->error_memcpy++;
            }
            else
            {
                lp_sde_da = DynaArray_insert(lp_da, lp_da->elements_num, &i);
                if (lp_sde_da == NULL_POINTER)
                {
                    lp_sde->lp_exception->error_malloc++;
                    lp_sde->status = False;
                }
                if (lp_sde_da->status == False)
                {
                    lp_sde->lp_exception->error_null_pointer += lp_sde_da->lp_exception->error_null_pointer;
                    lp_sde->lp_exception->error_malloc += lp_sde_da->lp_exception->error_malloc;
                    lp_sde->lp_exception->error_index_out += lp_sde_da->lp_exception->error_index_out;
                    lp_sde->lp_exception->error_realloc += lp_sde_da->lp_exception->error_realloc;
                    lp_sde->lp_exception->error_memset += lp_sde_da->lp_exception->error_memset;
                    lp_sde->lp_exception->error_memove += lp_sde_da->lp_exception->error_memove;
                    lp_sde->lp_exception->error_memcpy += lp_sde_da->lp_exception->error_memcpy;
                    lp_sde->status = False;
                }

                StatusDataException_free(lp_sde_da);
            }
        }
        lp_iter = lp_iter->next;
    }
    lp_sde->data = lp_da;
    if (lp_da->elements_num == 0)
        lp_sde->status = False;
    return lp_sde;
}
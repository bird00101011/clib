#include <error.h>
#include <dynaarray.h>
#include <linkedlist.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

int LinkedList_init(LPLinkedList lp_ll,
                    long ele_size,
                    int (*copy_func)(void *, void *),
                    int (*compare_func)(void *, void *),
                    int (*free_func)(void *))
{
    if (lp_ll == NULL_POINTER || ele_size <= 0)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    lp_ll->lp_tail = NULL_POINTER;
    lp_ll->lp_head = NULL_POINTER;
    lp_ll->ele_size = ele_size;
    lp_ll->copy_func = copy_func;
    lp_ll->compare_func = compare_func;
    lp_ll->free_func = free_func;
    lp_ll->eles_num = 0;

    return TRUE;
}

int LinkedList_free(LPLinkedList lp_ll)
{
    if (lp_ll == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }
    LPLinkedListNode lp_lln, lp_llt;
    lp_lln = lp_ll->lp_head;
    for (long i = 0; i < lp_ll->eles_num; i++)
    {
        if (lp_ll->free_func == NULL_POINTER)
            free(lp_lln->ele);
        else
        {
            if (lp_ll->free_func(lp_lln->ele) == FALSE)
            {
                set_last_error(CLIB_CALLBACKFUNC_FAILED);
                return FALSE;
            }
        }

        lp_llt = lp_lln->next;
        free(lp_lln);
        lp_lln = lp_llt;
    }

    return TRUE;
}

int LinkedList_insert(LPLinkedList lp_ll, long pos, void *ele)
{
    if (lp_ll == NULL_POINTER || ele == NULL_POINTER || pos < 0 || pos > lp_ll->eles_num)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    LPLinkedListNode lp_lln, lp_llt, lp_llnn;
    lp_lln = lp_ll->lp_head;
    char *src, *dst;
    src = (char *)ele;

    lp_llnn = (LPLinkedListNode)malloc(sizeof(LinkedListNode));
    if (lp_llnn == NULL_POINTER)
    {
        set_last_error(CLIB_MALLOC_FAILED);
        return FALSE;
    }

    lp_llnn->ele = malloc(lp_ll->ele_size);
    dst = (char *)lp_llnn->ele;
    long i;

    if (lp_llnn->ele == NULL_POINTER)
    {
        set_last_error(CLIB_MALLOC_FAILED);
        free(lp_llnn);
        return FALSE;
    }

    if (lp_ll->copy_func == NULL_POINTER)
    {
        if (memcpy(dst, src, lp_ll->ele_size) == NULL_POINTER)
        {
            set_last_error(CLIB_MEMCPY_FAILED);
            free(lp_llnn->ele);
            free(lp_llnn);
            return FALSE;
        }
    }
    else
    {
        if (lp_ll->copy_func(dst, src) == FALSE)
        {
            set_last_error(CLIB_CALLBACKFUNC_FAILED);
            return FALSE;
        }
    }

    if (lp_ll->eles_num == 0)
    {
        lp_llnn->prev = lp_llnn;
        lp_llnn->next = lp_llnn;
        lp_ll->lp_head = lp_llnn;
        lp_ll->lp_tail = lp_llnn;
    }
    else
    {
        if (pos - abs(pos - lp_ll->eles_num) < 0)
        {
            lp_lln = lp_ll->lp_head;
            for (i = 1; i <= pos; i++)
                lp_lln = lp_lln->next;
        }
        else
        {
            lp_lln = lp_ll->lp_tail;
            for (i = lp_ll->eles_num - 2; i >= pos; i--)
                lp_lln = lp_lln->prev;
        }

        if (pos < lp_ll->eles_num)
        {
            lp_llt = lp_lln->prev;
            lp_lln->prev = lp_llnn;
            lp_llnn->next = lp_lln;
            lp_llnn->prev = lp_llt;

            if (pos > 0)
                lp_llt->next = lp_llnn;
            else
                lp_ll->lp_head = lp_llnn;
        }
        else
        {
            lp_lln->next = lp_llnn;
            lp_llnn->prev = lp_lln;
            lp_llnn->next = NULL_POINTER;
            lp_ll->lp_tail = lp_llnn;
        }
    }

    lp_ll->eles_num++;
    return TRUE;
}

int LinkedList_del_by_pos(LPLinkedList lp_ll, long pos);

int LinkedList_del_by_ele(LPLinkedList lp_ll, void *ele, LPDynaArray lp_poses);

int LinkedList_get_by_pos(LPLinkedList lp_ll, long pos, void *ele);

int LinkedList_get_pos_by_ele(LPLinkedList lp_ll, void *ele, LPDynaArray lp_poses);

int LinkedList_edit_by_pos(LPLinkedList lp_ll, long pos, void *ele);

int LinkedList_edit_by_ele(LPLinkedList lp_ll, void *old_ele, void *new_ele, LPDynaArray lp_poses);

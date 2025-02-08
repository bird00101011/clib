#include <error.h>
#include <dynaarray.h>
#include <linkedlist.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>

int LinkedList_init(LPLinkedList lp_ll,
                    long ele_size,
                    int (*copy_func)(void *, void *),
                    int (*compare_func)(void *, void *),
                    int (*free_func)(void *))
{
    assert(lp_ll != NULL_POINTER);
    if (ele_size <= 0)
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
    assert(lp_ll != NULL_POINTER);
    LPLinkedListNode lp_lln, lp_ll_nin;
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

        lp_ll_nin = lp_lln->next;
        free(lp_lln);
        lp_lln = lp_ll_nin;
    }

    return TRUE;
}

int LinkedList_insert(LPLinkedList lp_ll, long pos, void *ele)
{
    assert(lp_ll != NULL_POINTER && ele != NULL_POINTER);
    if (pos < 0 || pos > lp_ll->eles_num)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    LPLinkedListNode lp_lln, lp_ll_nin, lp_llnn;
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
        lp_lln = lp_ll->lp_head;
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
            lp_ll_nin = lp_lln->prev;
            lp_lln->prev = lp_llnn;
            lp_llnn->next = lp_lln;
            lp_llnn->prev = lp_ll_nin;

            if (pos > 0)
                lp_ll_nin->next = lp_llnn;
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

int LinkedList_del_by_pos(LPLinkedList lp_ll, long pos)
{
    assert(lp_ll != NULL_POINTER);
    if (pos < 0 || pos >= lp_ll->eles_num)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    LPLinkedListNode lp_lln, lp_ll_nin;

    if (lp_ll->eles_num == 0)
    {
        lp_lln = lp_ll->lp_head;
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

        free(lp_lln);
        lp_ll->lp_head = NULL_POINTER;
        lp_ll->lp_tail = NULL_POINTER;
    }
    else
    {
        long i;
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

        if (lp_ll->free_func != NULL_POINTER)
        {
            if (lp_ll->free_func(lp_lln->ele) == FALSE)
            {
                set_last_error(CLIB_CALLBACKFUNC_FAILED);
                return FALSE;
            }
        }

        free(lp_lln->ele);

        if (pos == 0)
        {
            lp_ll_nin = lp_lln->next;
            lp_ll_nin->prev = NULL_POINTER;
            lp_ll->lp_head = lp_ll_nin;
        }
        else if (pos == lp_ll->eles_num - 1)
        {
            lp_ll_nin = lp_lln->prev;
            lp_ll_nin->next = NULL_POINTER;
            lp_ll->lp_tail = lp_ll_nin;
        }
        else
        {
            lp_ll_nin = lp_lln->next;
            lp_ll_nin->prev = lp_lln->prev;
            lp_ll_nin->prev->next = lp_ll_nin;
        }

        free(lp_lln);
    }

    lp_ll->eles_num--;
    return TRUE;
}

int LinkedList_del_by_ele(LPLinkedList lp_ll, void *ele, LPDynaArray lp_poses)
{
    assert(lp_ll != NULL_POINTER && ele != NULL_POINTER);

    if (lp_ll->eles_num == 0)
        return FALSE;

    char eq = FALSE;
    long len = lp_ll->eles_num;
    LPLinkedListNode lp_lln, lp_ll_nin, lp_ll_nip;
    lp_lln = lp_ll->lp_head;

    for (long i = 0; i < len; i++)
    {
        if (lp_ll->compare_func != NULL_POINTER)
        {
            if (lp_ll->compare_func(lp_lln->ele, ele) == TRUE)
                eq = TRUE;
        }
        else
        {
            if (memcmp(lp_lln->ele, ele, lp_ll->ele_size) == 0)
                eq = TRUE;
        }

        lp_ll_nin = lp_lln->next;
        lp_ll_nip = lp_lln->prev;

        if (eq == TRUE)
        {
            if (lp_ll->free_func != NULL_POINTER)
            {
                if (lp_ll->free_func(lp_lln->ele) == FALSE)
                {
                    set_last_error(CLIB_CALLBACKFUNC_FAILED);
                    return FALSE;
                }
            }
            else
                free(lp_lln->ele);

            if (lp_ll_nip == NULL_POINTER)
            {
                lp_ll->lp_head = lp_ll_nin;
                lp_ll->lp_head->prev = NULL_POINTER;
            }

            if (lp_ll_nin == NULL_POINTER)
            {
                lp_ll->lp_tail = lp_ll_nip;
                lp_ll->lp_tail->next = NULL_POINTER;
            }

            if (lp_ll_nin != NULL_POINTER && lp_ll_nip != NULL_POINTER)
            {
                lp_ll_nin->prev = lp_ll_nip;
                lp_ll_nip->next = lp_ll_nin;
            }

            free(lp_lln);
            lp_ll->eles_num--;

            if (lp_poses != NULL_POINTER)
                if (FALSE == DynaArray_insert(lp_poses, lp_poses->eles_num, &i))
                    return FALSE;

            eq = FALSE;
        }

        lp_lln = lp_ll_nin;
    }

    return TRUE;
}

int LinkedList_get_by_pos(LPLinkedList lp_ll, long pos, void *ele)
{
    assert(lp_ll != NULL_POINTER && ele != NULL_POINTER);
    if (pos < 0 || pos >= lp_ll->eles_num)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    LPLinkedListNode lp_lln;
    long i;
    if (pos - abs(pos - lp_ll->eles_num) < 0)
    {
        for (i = 0, lp_lln = lp_ll->lp_head; i < pos; i++)
            lp_lln = lp_lln->next;
    }
    else
    {
        for (i = lp_ll->eles_num - 1, lp_lln = lp_ll->lp_tail; i > pos; i--)
            lp_lln = lp_lln->prev;
    }

    if (lp_ll->copy_func != NULL_POINTER)
    {
        if (lp_ll->copy_func(ele, lp_lln->ele) == FALSE)
        {
            set_last_error(CLIB_CALLBACKFUNC_FAILED);
            return FALSE;
        }
    }
    else
    {
        if (memcpy((char *)ele, (char *)lp_lln->ele, lp_ll->ele_size) == NULL_POINTER)
        {
            set_last_error(CLIB_MEMCPY_FAILED);
            return FALSE;
        }
    }

    return TRUE;
}

void *LinkedList_get_addr_by_pos(LPLinkedList lp_ll, long pos)
{
    assert(lp_ll != NULL_POINTER);
    if (pos < 0 || pos >= lp_ll->eles_num)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return NULL_POINTER;
    }

    LPLinkedListNode lp_lln;
    long i;
    if (pos - abs(pos - lp_ll->eles_num) < 0)
    {
        for (i = 0, lp_lln = lp_ll->lp_head; i < pos; i++)
            lp_lln = lp_lln->next;
    }
    else
    {
        for (i = lp_ll->eles_num - 1, lp_lln = lp_ll->lp_tail; i > pos; i--)
            lp_lln = lp_lln->prev;
    }

    return (void *)lp_lln->ele;
}

int LinkedList_get_pos_by_ele(LPLinkedList lp_ll, void *ele, LPDynaArray lp_poses)
{
    assert(lp_ll != NULL_POINTER && ele != NULL_POINTER && lp_poses != NULL_POINTER);

    LPLinkedListNode lp_lln = lp_ll->lp_head;
    char eq = FALSE;
    for (long i = 0; i < lp_ll->eles_num; i++, lp_lln = lp_lln->next)
    {
        if (lp_ll->compare_func != NULL_POINTER)
            eq = lp_ll->compare_func(ele, lp_lln->ele);
        else
        {
            if (memcmp((char *)ele, (char *)lp_lln->ele, lp_ll->ele_size) == 0)
                eq = TRUE;
        }
        if (eq == TRUE)
        {
            if (DynaArray_insert(lp_poses, lp_poses->eles_num, &i) == FALSE)
                return FALSE;

            eq = FALSE;
        }
    }

    return TRUE;
}

int LinkedList_edit_by_pos(LPLinkedList lp_ll, long pos, void *ele)
{
    assert(lp_ll != NULL_POINTER && ele != NULL_POINTER);

    LPLinkedListNode lp_lln;
    long i;
    if (pos - abs(pos - lp_ll->eles_num) < 0)
    {
        for (i = 0, lp_lln = lp_ll->lp_head; i < pos; i++)
            lp_lln = lp_lln->next;
    }
    else
    {
        for (i = lp_ll->eles_num - 1, lp_lln = lp_ll->lp_tail; i > pos; i--)
            lp_lln = lp_lln->prev;
    }

    if (lp_ll->copy_func != NULL_POINTER)
    {
        if (lp_ll->copy_func(lp_lln->ele, ele) == FALSE)
        {
            set_last_error(CLIB_CALLBACKFUNC_FAILED);
            return FALSE;
        }
    }
    else
    {
        if (memcpy((char *)lp_lln->ele, (char *)ele, lp_ll->ele_size) == NULL_POINTER)
        {
            set_last_error(CLIB_MEMCPY_FAILED);
            return FALSE;
        }
    }

    return TRUE;
}

int LinkedList_edit_by_ele(LPLinkedList lp_ll, void *old_ele, void *new_ele, LPDynaArray lp_poses)
{
    assert(lp_ll != NULL_POINTER && old_ele != NULL_POINTER && new_ele != NULL_POINTER);

    LPLinkedListNode lp_lln = lp_ll->lp_head;
    char eq = FALSE;
    for (long i = 0; i < lp_ll->eles_num; i++, lp_lln = lp_lln->next)
    {
        if (lp_ll->compare_func != NULL_POINTER)
            eq = lp_ll->compare_func(old_ele, lp_lln->ele);
        else
        {
            if (memcmp((char *)old_ele, (char *)lp_lln->ele, lp_ll->ele_size) == 0)
                eq = TRUE;
        }
        if (eq == TRUE)
        {
            if (lp_ll->copy_func != NULL_POINTER)
            {
                if (lp_ll->copy_func(lp_lln->ele, new_ele) == FALSE)
                {
                    set_last_error(CLIB_CALLBACKFUNC_FAILED);
                    return FALSE;
                }
            }
            else
            {
                if (memcpy((char *)lp_lln->ele, (char *)new_ele, lp_ll->ele_size) == 0)
                {
                    set_last_error(CLIB_MEMCPY_FAILED);
                    return FALSE;
                }
            }

            if (DynaArray_insert(lp_poses, lp_poses->eles_num, &i) == FALSE)
                return FALSE;

            eq = FALSE;
        }
    }

    return TRUE;
}

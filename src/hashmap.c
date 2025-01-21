#include <error.h>
#include <dynaarray.h>
#include <linkedlist.h>
#include <hashmap.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>

int _da_copy_func(void *dst, void *src)
{
    LPLinkedList lp_dst = (LPLinkedList)dst;
    LPLinkedList lp_src = (LPLinkedList)src;

    if (LinkedList_free(lp_dst) == FALSE)
        return FALSE;

    if (LinkedList_init(lp_dst, lp_dst->ele_size, lp_dst->copy_func, lp_dst->compare_func, lp_dst->free_func) == FALSE)
        return FALSE;

    LPLinkedListNode lp_lln = lp_src->lp_head;
    for (long i = 0; i < lp_src->eles_num; i++)
    {
        if (LinkedList_insert(lp_dst, lp_dst->eles_num, lp_lln->ele) == FALSE)
            return FALSE;

        lp_lln = lp_lln->next;
    }

    if (LinkedList_free(lp_src) == FALSE)
        return FALSE;

    free(lp_src);
    return TRUE;
}

int _da_compare_func(void *dst, void *src)
{
    LPLinkedList lp_dst = (LPLinkedList)dst;
    LPLinkedList lp_src = (LPLinkedList)src;

    if (lp_dst->eles_num != lp_src->eles_num || lp_dst->ele_size != lp_src->ele_size)
        return FALSE;

    LPLinkedListNode lp_dst_lln = lp_dst->lp_head;
    LPLinkedListNode lp_src_lln = lp_src->lp_head;
    for (long i = 0; i < lp_dst->eles_num; i++)
    {
        if (lp_dst->compare_func != NULL_POINTER)
        {
            if (lp_dst->compare_func(lp_dst_lln->ele, lp_src_lln->ele) == FALSE)
                return FALSE;
        }
        lp_dst_lln = lp_dst_lln->next;
        lp_src_lln = lp_src_lln->next;
    }

    return TRUE;
}

int _da_free_func(void *dst)
{
    LPLinkedList lp_ll = (LPLinkedList)dst;
    if (LinkedList_free(lp_ll) == FALSE)
        return FALSE;

    return TRUE;
}

int HashMap_init(LPHashMap lp_hm,
                 int (*copy_func)(void *, void *),
                 int (*compare_func)(void *, void *),
                 int (*free_func)(void *))
{
    assert(lp_hm != NULL_POINTER);

    lp_hm->copy_func = copy_func;
    lp_hm->compare_func = compare_func;
    lp_hm->free_func = free_func;

    lp_hm->lp_lls = (LPDynaArray)malloc(sizeof(DynaArray));
    if (lp_hm->lp_lls == NULL_POINTER)
    {
        set_last_error(CLIB_MALLOC_FAILED);
        return FALSE;
    }

    if (DynaArray_init(lp_hm->lp_lls, 16, sizeof(LinkedList), _da_copy_func, _da_compare_func, _da_free_func) == FALSE)
    {
        free(lp_hm->lp_lls);
        return FALSE;
    }

    LinkedList ll;
    if (LinkedList_init(&ll, sizeof(HashMapKV), lp_hm->copy_func, lp_hm->compare_func, lp_hm->free_func) == FALSE)
        return FALSE;

    for (char i = 0; i < 16; i++)
    {
        if (DynaArray_insert(lp_hm->lp_lls, i, &ll) == FALSE)
        {
            DynaArray_free(lp_hm->lp_lls);
            return FALSE;
        }
    }

    return TRUE;
}

int HashMap_free(LPHashMap lp_map)
{
    assert(lp_hm != NULL_POINTER);

    if (DynaArray_free(lp_map->lp_lls) == FALSE)
        return FALSE;

    free(lp_map->lp_lls);
    return TRUE;
}

int HashMap_gen_hash_code(LPHashMap lp_map, void *key, long key_size, long *lp_hsc)
{
    assert(lp_map != NULL_POINTER && key != NULL_POINTER && lp_hsc != NULL_POINTER);

    if (key_size <= 0)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    long sum = 0;
    char *arr = (char *)key;
    for (long i = 0; i < key_size; i++)
        sum += *(unsigned char *)(arr + i);

    long hsc = sum % lp_map->lp_lls->eles_num;
    if (memcpy(lp_hsc, (long *)&hsc, 1) == NULL_POINTER)
    {
        set_last_error(CLIB_MEMCPY_FAILED);
        return FALSE;
    }

    return TRUE;
}

int HashMap_put(LPHashMap lp_map, void *key, long key_size, void *value, long value_size)
{
    assert(lp_map != NULL_POINTER || key != NULL_POINTER || value != NULL_POINTER);

    if (key_size <= 0 || value_size <= 0)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    long pos;
    if (FALSE == HashMap_gen_hash_code(lp_map, key, key_size, &pos))
        return FALSE;

    LinkedList ll = {};
    if (DynaArray_get_by_pos(lp_map->lp_lls, pos, &ll) == FALSE)
        return FALSE;

    HashMapKV src = {key, value, key_size, value_size};
    char eq = FALSE;

    if (ll.eles_num > 0)
    {
        LPLinkedListNode lp_lln = ll.lp_head;
        LPHashMapKV dst = (LPHashMapKV)lp_lln->ele;

        for (long i = 0; i < ll.eles_num; i++)
        {
            if (ll.compare_func(dst, &src) == TRUE)
            {
                eq = TRUE;
                if (ll.copy_func(dst, &src) == FALSE)
                {
                    set_last_error(CLIB_CALLBACKFUNC_FAILED);
                    return FALSE;
                }
                break;
            }

            lp_lln = lp_lln->next;
            dst = (LPHashMapKV)lp_lln->ele;
        }
    }

    if (eq == FALSE)
        if (LinkedList_insert(&ll, ll.eles_num, &src) == FALSE)
            return FALSE;

    return TRUE;
}

int HashMap_get(LPHashMap lp_map, LPHashMapKV *ele)
{
    assert(lp_map != NULL_POINTER && ele != NULL_POINTER);
    if (key_size <= 0)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    long pos;
    if (HashMap_gen_hash_code(lp_map, key, key_size, &pos) == FALSE)
        return FALSE;

    LinkedList ll = {};
    if (DynaArray_get_by_pos(lp_map->lp_lls, pos, &ll) == FALSE)
        return FALSE;

    if (ll.eles_num > 0)
    {
        LPLinkedListNode lp_lln = ll.lp_head;
        for (long i = 0; i < ll.eles_num; i++)
        {
            if (ll.compare_func(lp_lln->ele, ele) == TRUE)
                if (ll.copy_func != NULL_POINTER)
                    return ll.copy_func(ele, lp_lln->ele);

            lp_lln = lp_lln->next;
        }
    }

    return FALSE;
}

int HashMap_delete(LPHashMap lp_map, void *key, long key_size)
{
    assert(lp_map != NULL_POINTER && key != NULL_POINTER);
    if (key_size <= 0)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    long pos;
    if (HashMap_gen_hash_code(lp_map, key, key_size, &pos) == FALSE)
        return FALSE;

    LinkedList ll = {};
    if (DynaArray_get_by_pos(lp_map->lp_lls, pos, &ll) == FALSE)
        return FALSE;

    if (ll.eles_num > 0)
    {
        LPLinkedListNode lp_lln = ll.lp_head;
        for (long i = 0; i < ll.eles_num; i++)
        {
            if (ll.compare_func(lp_lln->ele, ele) == TRUE)
                if (ll.free_func != NULL_POINTER)
                    return ll.free_func(lp_lln->ele);

            lp_lln = lp_lln->next;
        }
    }

    return FALSE;
}
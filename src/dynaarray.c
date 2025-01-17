#include <error.h>
#include <dynaarray.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

int DynaArray_init(LPDynaArray lp_da,
                   long capacity,
                   long ele_size,
                   int (*copy_func)(void *, void *),
                   int (*compare_func)(void *, void *),
                   int (*free_func)(void *))
{
    if (lp_da == NULL_POINTER || capacity <= 0 || ele_size <= 0)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }
    lp_da->capacity = capacity;
    lp_da->ele_size = ele_size;
    lp_da->copy_func = copy_func;
    lp_da->compare_func = compare_func;
    lp_da->free_func = free_func;
    lp_da->eles_num = 0;

    lp_da->eles = malloc(ele_size * capacity);
    if (lp_da->eles == NULL_POINTER)
    {
        set_last_error(CLIB_MALLOC_FAILED);
        return FALSE;
    }

    if (memset(lp_da->eles, 0, capacity * lp_da->ele_size) == NULL_POINTER)
    {
        set_last_error(CLIB_MEMSET_FAILED);
        free(lp_da->eles);
        return FALSE;
    }

    return TRUE;
}

int DynaArray_free(LPDynaArray lp_da)
{
    if (lp_da == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    if (lp_da->eles != NULL_POINTER)
    {
        if (lp_da->free_func != NULL_POINTER)
        {
            char *src;
            char *arr = (char *)lp_da->eles;
            for (long i = 0; i < lp_da->eles_num; i++)
            {
                src = arr + i * lp_da->ele_size;
                if (lp_da->free_func(src) == FALSE)
                {
                    set_last_error(CLIB_CALLBACKFUNC_FAILED);
                    return FALSE;
                }
            }
        }
        free(lp_da->eles);
    }

    return TRUE;
}

int DynaArray_reallocate(LPDynaArray lp_da, long new_capacity)
{
    if (lp_da == NULL_POINTER || new_capacity <= 0)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    void *eles = realloc(lp_da->eles, new_capacity * lp_da->ele_size);
    if (eles == NULL_POINTER)
    {
        set_last_error(CLIB_REALLOC_FAILED);
        return FALSE;
    }

    lp_da->eles = eles;
    return TRUE;
}

int DynaArray_insert(LPDynaArray lp_da, long pos, void *ele)
{
    if (lp_da == NULL_POINTER || ele == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    if (pos < 0 || pos > lp_da->eles_num)
    {
        set_last_error(CLIB_INDEX_OUT_FAILED);
        return FALSE;
    }

    if (lp_da->eles_num == lp_da->capacity)
    {
        if (DynaArray_reallocate(lp_da, lp_da->capacity * 2) == FALSE)
            return FALSE;
    }

    char *arr = (char *)lp_da->eles;
    char *elec = (char *)ele;
    if (lp_da->eles_num == 0)
    {
        if (lp_da->copy_func == NULL_POINTER)
        {
            if (memcpy(arr, elec, lp_da->ele_size) == NULL_POINTER)
            {
                set_last_error(CLIB_MEMCPY_FAILED);
                return FALSE;
            }
        }
        else
        {
            if (lp_da->copy_func(arr, elec) == FALSE)
            {
                set_last_error(CLIB_CALLBACKFUNC_FAILED);
                return FALSE;
            }
        }
    }
    else
    {
        char *dst = arr + (pos + 1) * lp_da->ele_size;
        char *src = arr + pos * lp_da->ele_size;
        char *end = arr + lp_da->eles_num * lp_da->ele_size;
        size_t count = lp_da->ele_size * (lp_da->eles_num - pos);

        if (memmove(dst, src, count) == NULL_POINTER)
        {
            set_last_error(CLIB_MEMOVE_FAILED);
            return FALSE;
        }

        if (lp_da->copy_func == NULL_POINTER)
        {
            if (memcpy(src, elec, lp_da->ele_size) == NULL_POINTER)
            {
                set_last_error(CLIB_MEMCPY_FAILED);

                if (memmove(src, dst, count) == NULL_POINTER)
                    set_last_error(CLIB_MEMOVE_FAILED);

                return FALSE;
            }
        }
        else
        {
            if (lp_da->copy_func(src, elec) == FALSE)
            {
                set_last_error(CLIB_CALLBACKFUNC_FAILED);

                if (memmove(src, dst, count) == NULL_POINTER)
                    set_last_error(CLIB_MEMOVE_FAILED);

                return FALSE;
            }
        }
    }

    lp_da->eles_num++;
    return TRUE;
}

int DynaArray_del_by_pos(LPDynaArray lp_da, long pos)
{
    if (lp_da == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    if (pos < 0 || pos >= lp_da->eles_num)
    {
        set_last_error(CLIB_INDEX_OUT_FAILED);
        return FALSE;
    }

    char *arr = (char *)lp_da->eles;
    char *dst = arr + pos * lp_da->ele_size;

    if (lp_da->free_func == NULL_POINTER)
    {
        if (lp_da->free_func(dst) == FALSE)
        {
            set_last_error(CLIB_CALLBACKFUNC_FAILED);
            return FALSE;
        }
    }

    if (lp_da->eles_num > 1)
    {
        char *src = arr + (pos + 1) * lp_da->ele_size;
        size_t mv_count = (lp_da->eles_num - pos - 1) * lp_da->ele_size;

        if (memmove(dst, src, mv_count) == NULL_POINTER)
        {
            set_last_error(CLIB_MEMOVE_FAILED);
            return FALSE;
        }
    }

    char *end = arr + (lp_da->eles_num - 1) * lp_da->ele_size;

    if (memset(end, 0, lp_da->ele_size) == NULL_POINTER)
        set_last_error(CLIB_MEMSET_FAILED);

    lp_da->eles_num--;
    return TRUE;
}

int DynaArray_del_by_ele(LPDynaArray lp_da, void *ele)
{
    if (lp_da == NULL_POINTER || ele == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    char *arr = (char *)lp_da->eles;
    char *dst = (char *)ele;
    char *src;
    char eq = FALSE;
    for (long i = 0; i < lp_da->eles_num; i++)
    {
        src = arr + i * lp_da->ele_size;
        if (lp_da->compare_func == NULL_POINTER)
        {
            if (memcmp(src, dst, lp_da->ele_size) == 0)
                eq = TRUE;
        }
        else
            eq = lp_da->compare_func(src, dst);

        if (eq == TRUE)
        {
            if (DynaArray_del_by_pos(lp_da, i) == FALSE)
                return FALSE;
            i--;
        }

        eq = FALSE;
    }

    return TRUE;
}

int DynaArray_edit_by_pos(LPDynaArray lp_da, long pos, void *ele)
{
    if (lp_da == NULL_POINTER || pos < 0 || pos >= lp_da->eles_num || ele == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    char *arr = (char *)lp_da->eles;
    char *dst = arr + pos * lp_da->ele_size;
    char *src = (char *)ele;
    size_t count = lp_da->ele_size;
    if (lp_da->copy_func == NULL_POINTER)
    {
        if (memcpy(dst, src, count) == NULL_POINTER)
            return FALSE;
    }
    else
    {
        if (lp_da->copy_func(dst, src) == FALSE)
            return FALSE;
    }

    return TRUE;
}

int DynaArray_edit_by_ele(LPDynaArray lp_da, void *old_ele, void *new_ele, LPDynaArray lp_poses)
{
    if (lp_da == NULL_POINTER || old_ele == NULL_POINTER || new_ele == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    char *arr = (char *)lp_da->eles;
    char *oe = (char *)old_ele;
    char *ne = (char *)new_ele;
    char *src;
    char eq = FALSE;
    for (long i = 0; i < lp_da->eles_num; i++)
    {
        src = arr + i * lp_da->ele_size;
        if (lp_da->compare_func == NULL_POINTER)
        {
            if (memcmp(src, oe, lp_da->ele_size) == 0)
                eq = TRUE;
        }
        else
            eq = lp_da->compare_func(src, oe);

        if (eq == TRUE)
        {
            if (lp_da->copy_func == NULL_POINTER)
            {
                if (memcpy(src, new_ele, lp_da->ele_size) != 0)
                    return FALSE;
            }
            else
            {
                if (lp_da->copy_func(src, new_ele) == FALSE)
                    return FALSE;
            }

            if (lp_poses != NULL_POINTER)
            {
                if (DynaArray_insert(lp_poses, lp_poses->eles_num, &i) == FALSE)
                    return FALSE;
            }
        }

        eq = FALSE;
    }

    return TRUE;
}

int DynaArray_get_by_pos(LPDynaArray lp_da, long pos, void *ele)
{
    if (lp_da == NULL_POINTER || pos < 0 || pos >= lp_da->eles_num)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    char *arr = (char *)lp_da->eles;
    char *src = arr + pos * lp_da->ele_size;
    char *oe = (char *)ele;
    if (lp_da->copy_func == NULL_POINTER)
    {
        if (memcpy(oe, src, lp_da->ele_size) == NULL_POINTER)
            return FALSE;
    }
    else
    {
        if (lp_da->copy_func(oe, src) == FALSE)
            return FALSE;
    }

    return TRUE;
}

int DynaArray_get_pos_by_ele(LPDynaArray lp_da, void *ele, LPDynaArray lp_poses)
{
    if (lp_da == NULL_POINTER || ele == NULL_POINTER || lp_poses == NULL_POINTER)
    {
        set_last_error(CLIB_PARAMS_WRONG);
        return FALSE;
    }

    char *arr = (char *)lp_da->eles;
    char *src;
    char *oe = (char *)ele;
    char eq = FALSE;
    for (long i = 0; i < lp_da->eles_num; i++)
    {
        src = arr + i * lp_da->ele_size;
        if (lp_da->compare_func == NULL_POINTER)
        {
            if (memcmp(src, oe, lp_da->ele_size) == 0)
                eq = TRUE;
        }
        else
            eq = lp_da->compare_func(src, oe);

        if (eq == TRUE)
        {
            if (DynaArray_insert(lp_poses, lp_poses->eles_num, &i) == FALSE)
                return FALSE;
        }
        eq = FALSE;
    }

    return TRUE;
}
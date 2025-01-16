#include <error.h>
#include <dynaarray.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

int DynaArray_new(LPDynaArray lp_da,
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

    lp_da->eles = malloc(ele_size * capacity);
    if (lp_da->eles == NULL_POINTER)
    {
        set_last_error(CLIB_MALLOC_FAILED);
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

    lp_da = realloc(lp_da->eles, new_capacity);
    if (lp_da == NULL_POINTER)
    {
        set_last_error(CLIB_REALLOC_FAILED);
        return FALSE;
    }

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

    char *dst = (char *)lp_da->eles + (pos + 1) * lp_da->ele_size;
    char *src = (char *)lp_da->eles + pos * lp_da->ele_size;
    size_t count = lp_da->ele_size * (lp_da->eles_num - pos);
    if (memmove(dst, src, count) == NULL_POINTER)
    {
        set_last_error(CLIB_MEMOVE_FAILED);
        return FALSE;
    }

    int opr = TRUE;
    if (lp_da->copy_func == NULL_POINTER)
    {
        if (memcpy(src, (char *)ele, lp_da->ele_size) == NULL_POINTER)
        {
            set_last_error(CLIB_MEMCPY_FAILED);
            opr = FALSE;
        }
    }
    else
    {
        if (lp_da->copy_func(src, (char *)ele) == FALSE)
        {
            set_last_error(CLIB_CALLBACKFUNC_FAILED);
            opr = FALSE;
        }
    }

    if (opr == FALSE)
    {
        if (memmove(src, dst, count) == NULL_POINTER)
            set_last_error(CLIB_MEMOVE_FAILED);

        return FALSE;
    }

    lp_da->eles_num++;
    return TRUE;
}

int DynaArray_del_by_pos(LPDynaArray lp_da, long pos);

int DynaArray_del_by_ele(LPDynaArray lp_da, void *ele, LPDynaArray lp_poses);

int DynaArray_edit_by_pos(LPDynaArray lp_da, long pos, void *ele);

int DynaArray_edit_by_ele(LPDynaArray lp_da, void *old_ele, void *new_ele);

int DynaArray_get_by_pos(LPDynaArray lp_da, long pos);

int DynaArray_get_pos_by_ele(LPDynaArray lp_da, void *ele);
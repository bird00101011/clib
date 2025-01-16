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
            for (long i = 0; i < lp_da->eles_num; i++)
            {
                if (lp_da->free_func((char *)lp_da->eles + i * lp_da->ele_size) == FALSE)
                {
                    set_last_error(CLIB_CALLBACKFUNC_FAILED);
                    return FALSE;
                }
            }
        }
        free(lp_da->eles);
    }
    free(lp_da);

    return TRUE;
}

int DynaArray_reallocate(LPDynaArray lp_da, long new_capacity);

int DynaArray_insert(LPDynaArray lp_da, long pos, void *ele);

int DynaArray_del_by_pos(LPDynaArray lp_da, long pos);

int DynaArray_del_by_ele(LPDynaArray lp_da, void *ele, LPDynaArray lp_poses);

int DynaArray_edit_by_pos(LPDynaArray lp_da, long pos, void *ele);

int DynaArray_edit_by_ele(LPDynaArray lp_da, void *old_ele, void *new_ele);

int DynaArray_get_by_pos(LPDynaArray lp_da, long pos);

int DynaArray_get_pos_by_ele(LPDynaArray lp_da, void *ele);
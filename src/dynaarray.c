#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <dynaarray.h>

LPStatusDataException DynaArray_new(long capacity, long element_size)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (capacity <= 0 || element_size <= 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
        return lp_sde;
    }

    LPDynaArray lpAl = malloc(sizeof(DynaArray));
    if (lpAl == NULL_POINTER)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_malloc = True;
        return lp_sde;
    }
    lp_sde->data = lpAl;

    lpAl->elements_num = 0;
    lpAl->element_size = element_size;

    lpAl->capacity = capacity > 5 ? capacity : 5;

    lpAl->elements = malloc(lpAl->capacity * lpAl->element_size);
    if (lpAl->elements == NULL_POINTER)
    {
        free(lpAl);

        lp_sde->status = False;
        lp_sde->lp_exception->error_malloc = True;

        return lp_sde;
    }
    if (NULL_POINTER == memset(lpAl->elements, 0, lpAl->capacity * lpAl->element_size))
    {
        free(lpAl->elements);
        free(lpAl);
        lp_sde->status = False;
        lp_sde->lp_exception->error_memset = True;

        return lp_sde;
    }

    return lp_sde;
}

LPStatusDataException DynaArray_free(LPDynaArray lp_dyna_array)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array != NULL_POINTER)
    {
        if (lp_dyna_array->elements != NULL_POINTER)
        {
            free(lp_dyna_array->elements);
            lp_dyna_array->elements = NULL_POINTER; // 防止野指针
        }

        free(lp_dyna_array);
    }
    else
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        lp_sde->data = lp_dyna_array;
    }

    return lp_sde;
}

LPStatusDataException DynaArray_reallocate(LPDynaArray lp_dyna_array, long new_capacity)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    lp_sde->data = lp_dyna_array;

    if (new_capacity <= 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;

        return lp_sde;
    }

    long new_mem_size = new_capacity * lp_dyna_array->element_size;
    long old_mem_size = lp_dyna_array->capacity * lp_dyna_array->element_size;
    void *new_elements = realloc(lp_dyna_array->elements, new_mem_size);

    if (new_elements == NULL_POINTER)
    {
        lp_sde->data = lp_dyna_array;
        lp_sde->lp_exception->error_realloc = True;
        lp_sde->status = False;

        return lp_sde;
    }

    lp_dyna_array->elements = new_elements;

    if (lp_dyna_array->capacity < new_capacity)
    {
        if (NULL_POINTER == memset((char *)lp_dyna_array->elements + lp_dyna_array->capacity * lp_dyna_array->element_size,
                                   0, new_mem_size - old_mem_size))
        {
            new_elements = realloc(lp_dyna_array->elements, lp_dyna_array->capacity);
            if (new_elements == NULL_POINTER)
            {
                lp_sde->lp_exception->error_realloc = True;
                lp_sde->status = False;

                return lp_sde;
            }

            lp_sde->lp_exception->error_memset = True;
            lp_sde->status = False;

            return lp_sde;
        }
    }

    lp_dyna_array->capacity = new_capacity;
    if (lp_dyna_array->elements_num > lp_dyna_array->capacity)
        lp_dyna_array->elements_num = lp_dyna_array->capacity;

    return lp_sde;
}

LPStatusDataException DynaArray_insert(LPDynaArray lp_dyna_array, long position, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (position >= 0 && position <= lp_dyna_array->elements_num)
    {
        char sig = 0;
        if (lp_dyna_array->elements_num == lp_dyna_array->capacity)
        {
            sig = 1;
            LPStatusDataException ar_lp_sde = DynaArray_reallocate(lp_dyna_array, 2 * lp_dyna_array->capacity);
            if (ar_lp_sde->status != True)
            {
                StatusDataException_free(lp_sde);
                return ar_lp_sde;
            }

            StatusDataException_free(ar_lp_sde);
        }

        if (position < lp_dyna_array->elements_num)
        {
            if (NULL_POINTER == memmove(((char *)lp_dyna_array->elements) + (position + 1) * lp_dyna_array->element_size,
                                        (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size,
                                        (lp_dyna_array->elements_num - position) * lp_dyna_array->element_size))
            {
                lp_sde->status = False;
                lp_sde->lp_exception->error_memove = True;

                return lp_sde;
            }

            if (NULL_POINTER == memcpy((char *)lp_dyna_array->elements + position * lp_dyna_array->element_size,
                                       element, lp_dyna_array->element_size))
            {
                // TODO 恢复之前被移动的元素内存，或者抛出异常
                lp_sde->status = False;
                lp_sde->lp_exception->error_memcpy = True;

                return lp_sde;
            }
        }
        else // position == elements_num
        {
            if (NULL_POINTER == memcpy((char *)lp_dyna_array->elements + position * lp_dyna_array->element_size,
                                       element, lp_dyna_array->element_size))
            {
                if (sig == 1)
                {
                    LPStatusDataException ar_lp_sde = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
                    if (ar_lp_sde->status != True)
                    {
                        StatusDataException_free(lp_sde);
                        return ar_lp_sde;
                    }
                    StatusDataException_free(ar_lp_sde);
                }
                else
                {
                    lp_sde->lp_exception->error_memcpy = True;
                    lp_sde->status = False;
                    return lp_sde;
                }
            }
        }
    }
    else
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;

        return lp_sde;
    }

    lp_dyna_array->elements_num = lp_dyna_array->elements_num + 1; // 也可以用++
    lp_sde->data = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;

    return lp_sde;
}

LPStatusDataException DynaArray_delete_by_position(LPDynaArray lp_dyna_array, long position)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;

        return lp_sde;
    }

    if (lp_dyna_array->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;

        return lp_sde;
    }

    if (lp_dyna_array->elements_num < lp_dyna_array->capacity / 2)
    {
        LPStatusDataException rsde = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
        if (rsde->status == False)
        {
            StatusDataException_free(lp_sde);
            return rsde;
        }
    }

    if (position >= 0 && position < lp_dyna_array->elements_num)
    {
        if (lp_dyna_array->elements_num > 1)
        {
            if (NULL_POINTER == memmove(((char *)lp_dyna_array->elements) + position * lp_dyna_array->element_size,
                                        (char *)lp_dyna_array->elements + (position + 1) * lp_dyna_array->element_size,
                                        (lp_dyna_array->elements_num - position - 1) * lp_dyna_array->element_size))
            {
                lp_sde->status = False;
                lp_sde->lp_exception->error_memove = True;

                return lp_sde;
            }
        }

        if (NULL_POINTER == memset((char *)lp_dyna_array->elements + (lp_dyna_array->elements_num - 1) * lp_dyna_array->element_size,
                                   0, lp_dyna_array->element_size))
        {
            // TODO 恢复之前被移动的元素内存，或者抛出异常
            lp_sde->status = False;
            lp_sde->lp_exception->error_memset = True;

            return lp_sde;
        }
    }
    else
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;

        return lp_sde;
    }

    lp_dyna_array->elements_num--;
    lp_sde->status = True;

    return lp_sde;
}

LPStatusDataException DynaArray_get_by_position(LPDynaArray lp_dyna_array, long position)
{
    LPStatusDataException sde = StatusDataException_new();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_dyna_array == NULL_POINTER)
    {
        sde->lp_exception->error_null_pointer = True;
        sde->status = False;
        return sde;
    }

    if (position >= 0 && position < lp_dyna_array->elements_num)
    {
        sde->data = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
    }
    else
    {
        sde->lp_exception->error_index_out = True;
        sde->status = False;
    }

    return sde;
}

LPStatusDataException DynaArray_edit_by_position(LPDynaArray lp_dyna_array, long position, Object element)
{
    LPStatusDataException sde = StatusDataException_new();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_dyna_array == NULL_POINTER || element == NULL_POINTER)
    {
        sde->lp_exception->error_null_pointer = True;
        sde->status = False;

        return sde;
    }

    if (position >= 0 && position < lp_dyna_array->elements_num)
    {
        if (NULL_POINTER == memcpy((char *)lp_dyna_array->elements + position * lp_dyna_array->element_size,
                                   element, lp_dyna_array->element_size))
        {
            sde->status = False;
            sde->lp_exception->error_memcpy = True;
        }
        else
            sde->data = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
    }
    else
    {
        sde->status = False;
        sde->lp_exception->error_index_out = True;
    }

    return sde;
}

LPStatusDataException DynaArray_edit_by_element(LPDynaArray lp_dyna_array, Object old_element, Object new_element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || old_element == NULL_POINTER || new_element == NULL_POINTER)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_null_pointer = True;
        return lp_sde;
    }

    int ls = sizeof(long);
    LPStatusDataException lp_sde_pos = DynaArray_new(10, ls);
    if (lp_sde_pos == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_sde_pos->status == False)
    {
        StatusDataException_free(lp_sde);
        return lp_sde_pos;
    }

    StatusDataException_free(lp_sde);
    LPDynaArray lp_da = (LPDynaArray)lp_sde_pos->data;

    LPStatusDataException lp_sde_iter;
    for (long i = 0; i < lp_dyna_array->elements_num; i++)
    {
        lp_sde_iter = DynaArray_get_by_position(lp_dyna_array, i);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde->lp_exception->error_null_pointer = True;
            lp_sde->status = False;
            break;
        }
        if (lp_sde_iter->status == False)
        {
            StatusDataException_free(lp_sde);
            return lp_sde_iter;
        }
        if (memcmp((char *)lp_sde_iter->data, (char *)old_element, lp_dyna_array->element_size) == 0)
        {
            StatusDataException_free(lp_sde_iter);
            lp_sde_iter = DynaArray_edit_by_position(lp_dyna_array, i, new_element);
            if (lp_sde_iter == NULL_POINTER || lp_sde_iter->status == False)
                lp_sde_pos->lp_exception->error_some++;
            else
            {
                StatusDataException_free(lp_sde_iter);
                lp_sde_iter = DynaArray_insert(lp_da, lp_da->elements_num, &i);
                if (lp_sde_iter == NULL_POINTER || lp_sde_iter->status == False)
                    lp_sde_pos->lp_exception->error_some++;
            }
        }
        StatusDataException_free(lp_sde_iter);
    }
    if (lp_sde_pos->lp_exception->error_some != 0)
        lp_sde_pos->status = False;
    return lp_sde_pos;
}

LPStatusDataException DynaArray_delete_by_element(LPDynaArray lp_dyna_array, Object old_element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || old_element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        lp_sde->data = NULL_POINTER;

        return lp_sde;
    }

    lp_sde->data = lp_dyna_array;

    if (lp_dyna_array->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;

        return lp_sde;
    }

    LPStatusDataException lp_sde_iter;
    for (long i = 0; i < lp_dyna_array->elements_num; i++)
    {
        lp_sde_iter = DynaArray_get_by_position(lp_dyna_array, i);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde->lp_exception->error_null_pointer = True;
            lp_sde->status = False;
            break;
        }
        if (lp_sde_iter->status == False)
        {
            StatusDataException_free(lp_sde);
            return lp_sde_iter;
        }
        if (memcmp((char *)lp_sde_iter->data, (char *)old_element, lp_dyna_array->element_size) == 0)
        {
            StatusDataException_free(lp_sde_iter);
            lp_sde_iter = DynaArray_delete_by_position(lp_dyna_array, i);
            if (lp_sde_iter == NULL_POINTER || lp_sde_iter->status == False)
                lp_sde->lp_exception->error_some++;
            else
                i--;
        }
        StatusDataException_free(lp_sde_iter);
    }

    if (lp_sde->lp_exception->error_some != 0)
        lp_sde->status = False;

    return lp_sde;
}

LPStatusDataException DynaArray_get_position_by_element(LPDynaArray lp_dyna_array, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        lp_sde->data = NULL_POINTER;

        return lp_sde;
    }

    if (lp_dyna_array->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;

        return lp_sde;
    }

    int long_size = sizeof(long);
    LPStatusDataException lp_sde_pos = DynaArray_new(10, long_size);
    if (lp_sde_pos == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        lp_sde->data = lp_dyna_array;
        return lp_sde;
    }
    if (lp_sde_pos->status == False)
    {
        StatusDataException_free(lp_sde);
        return lp_sde_pos;
    }
    StatusDataException_free(lp_sde);
    LPDynaArray lp_dn_l = (LPDynaArray)lp_sde_pos->data;

    LPStatusDataException lp_sde_iter;
    for (long i = 0; i < lp_dyna_array->elements_num; i++)
    {
        lp_sde_iter = DynaArray_get_by_position(lp_dyna_array, i);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde_pos->lp_exception->error_some++;
            continue;
        }
        if (lp_sde_iter->status == False)
        {
            lp_sde_pos->lp_exception->error_some++;
            StatusDataException_free(lp_sde_iter);
            continue;
        }
        if (memcmp((char *)lp_sde_iter->data, (char *)element, lp_dyna_array->element_size) == 0)
        {
            StatusDataException_free(lp_sde_iter);
            lp_sde_iter = DynaArray_insert(lp_dn_l, lp_dn_l->elements_num, &i);

            if (lp_sde_iter == NULL_POINTER)
                lp_sde_pos->lp_exception->error_some++;
            if (lp_sde_iter->status == False)
                lp_sde_pos->lp_exception->error_some++;
        }
        StatusDataException_free(lp_sde_iter);
    }

    return lp_sde_pos;
}
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <dynaarray.h>

/**
 * Create a new dynamic array instance
 *
 * params:
 *  long capacity: array capacity
 *  long element_size: The number of bytes of the element
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_index_out, error_malloc, error_memset]
 */
LPStatusDataException DynaArray_new(long capacity, long element_size)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (capacity <= 0 || element_size <= 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    LPDynaArray lpAl = malloc(sizeof(DynaArray));
    if (lpAl == NULL_POINTER)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_malloc++;
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
        lp_sde->lp_exception->error_malloc++;

        return lp_sde;
    }
    if (NULL_POINTER == memset(lpAl->elements, 0, lpAl->capacity * lpAl->element_size))
    {
        free(lpAl->elements);
        free(lpAl);
        lp_sde->status = False;
        lp_sde->lp_exception->error_memset++;

        return lp_sde;
    }

    return lp_sde;
}

/**
 * Release a dynamic array
 *
 * params:
 *  LPDynaArray lp_dyna_array: dynamic array pointers
 *  Boolean (*func)(object): the property releases the function pointer
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_malloc, error_index_out, error_callback, error_null_pointer]
 */
LPStatusDataException DynaArray_free(LPDynaArray lp_dyna_array, Boolean (*func)(Object))
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array != NULL_POINTER)
    {
        if (lp_dyna_array->elements != NULL_POINTER)
        {
            LPStatusDataException lp_sde_iter;
            long z = lp_dyna_array->elements_num;
            for (long i = 0; i < z; i++)
            {
                lp_sde_iter = DynaArray_get_by_position(lp_dyna_array, i);
                if (lp_sde_iter == NULL_POINTER)
                {
                    lp_sde->lp_exception->error_malloc += lp_sde_iter->lp_exception->error_malloc;
                    lp_sde->status = False;
                    continue;
                }
                if (lp_sde_iter->status == False)
                {
                    lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
                    lp_sde->status = False;
                    continue;
                }
                if (func != NULL_POINTER)
                {
                    if (False == func(lp_sde_iter->data))
                    {
                        lp_sde->lp_exception->error_callback++;
                        lp_sde->status = False;
                    }
                }
                free(lp_sde_iter->data);

                lp_dyna_array->element_num--;
            }

            free(lp_dyna_array->elements);
        }

        free(lp_dyna_array);
    }
    else
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        lp_sde->data = lp_dyna_array;
    }

    return lp_sde;
}

/**
 * Expand the capacity of dynamic arrays
 *
 * params:
 *  LPDynaArray lp_dyna_array: dynamic array pointers
 *  long new_capacity: new capacity
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_malloc, error_index_out, error_realloc, error_memset]
 */
LPStatusDataException DynaArray_reallocate(LPDynaArray lp_dyna_array, long new_capacity)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_sde->data = lp_dyna_array;

    if (new_capacity <= 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    long new_mem_size = new_capacity * lp_dyna_array->element_size;
    long old_mem_size = lp_dyna_array->capacity * lp_dyna_array->element_size;
    void *new_elements = realloc(lp_dyna_array->elements, new_mem_size);

    if (new_elements == NULL_POINTER)
    {
        lp_sde->lp_exception->error_realloc++;
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
                lp_sde->lp_exception->error_realloc++;

            lp_sde->lp_exception->error_memset++;
            lp_sde->status = False;
            return lp_sde;
        }
    }

    lp_dyna_array->capacity = new_capacity;
    if (lp_dyna_array->elements_num > lp_dyna_array->capacity)
        lp_dyna_array->elements_num = lp_dyna_array->capacity;

    return lp_sde;
}

/**
 * Insert a new element in a dynamic array
 *
 * params:
 *  LPDynaArray lp_dyna_array: dyna array pointer
 *  long position: the position in the dynamic array where you want to insert the element,
 *                 if the position is the number of inserted elements, insert to the end
 *                 of the inserted elements, otherwise inserts in front of the element in
 *                 its original current position.
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_malloc, error_index_out, error_realloc, error_memset,
 *              error_memove, error_memcpy]
 */
LPStatusDataException DynaArray_insert(LPDynaArray lp_dyna_array, long position, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
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
                lp_sde->lp_exception->error_malloc += ar_lp_sde->lp_exception->error_malloc;
                lp_sde->lp_exception->error_index_out += ar_lp_sde->lp_exception->error_index_out;
                lp_sde->lp_exception->error_realloc += ar_lp_sde->lp_exception->error_realloc;
                lp_sde->lp_exception->error_memset += ar_lp_sde->lp_exception->error_memset;
                StatusDataException_free(ar_lp_sde);
                return lp_sde;
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
                lp_sde->lp_exception->error_memove++;
                return lp_sde;
            }

            if (NULL_POINTER == memcpy((char *)lp_dyna_array->elements + position * lp_dyna_array->element_size,
                                       element, lp_dyna_array->element_size))
            {
                // TODO 恢复之前被移动的元素内存，或者抛出异常
                lp_sde->status = False;
                lp_sde->lp_exception->error_memcpy++;
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
                        lp_sde->lp_exception->error_malloc += ar_lp_sde->lp_exception->error_malloc;
                        lp_sde->lp_exception->error_index_out += ar_lp_sde->lp_exception->error_index_out;
                        lp_sde->lp_exception->error_realloc += ar_lp_sde->lp_exception->error_realloc;
                        lp_sde->lp_exception->error_memset += ar_lp_sde->lp_exception->error_memset;
                        StatusDataException_free(ar_lp_sde);
                        return lp_sde;
                    }
                    StatusDataException_free(ar_lp_sde);
                }
                else
                {
                    lp_sde->lp_exception->error_memcpy++;
                    lp_sde->status = False;
                    return lp_sde;
                }
            }
        }
    }
    else
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;

        return lp_sde;
    }

    lp_dyna_array->elements_num++;
    lp_sde->data = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;

    return lp_sde;
}

/**
 * Elements are deleted based on their position in the dynamic array
 *
 * params:
 *  LPDynaArray lp_dyna_array: dyna array pointer
 *  long position: the position of the element in the dynamic array to be removed
 *  Boolean (*func)(Object): function pointer that frees the memory of a subproperty
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_malloc, error_index_out, error_realloc, error_memset, error_callback,
 *              error_memove]
 */
LPStatusDataException DynaArray_delete_by_position(LPDynaArray lp_dyna_array, long position, Boolean (*func)(Object))
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_dyna_array->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_dyna_array->elements_num < lp_dyna_array->capacity / 2)
    {
        LPStatusDataException ar_lp_sde = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
        if (ar_lp_sde == NULL_POINTER)
        {
            lp_sde->lp_exception->error_malloc++;
            lp_sde->status = False;
            return lp_sde;
        }
        if (ar_lp_sde->status == False)
        {
            lp_sde->lp_exception->error_malloc += ar_lp_sde->lp_exception->error_malloc;
            lp_sde->lp_exception->error_index_out += ar_lp_sde->lp_exception->error_index_out;
            lp_sde->lp_exception->error_realloc += ar_lp_sde->lp_exception->error_realloc;
            lp_sde->lp_exception->error_memset += ar_lp_sde->lp_exception->error_memset;
            StatusDataException_free(ar_lp_sde);
            lp_sde->status = False;
            return lp_sde;
        }
    }

    if (position >= 0 && position < lp_dyna_array->elements_num)
    {
        if (func != NULL_POINTER)
        {
            if (True != func((char *)lp_dyna_array->elements + position * lp_dyna_array->element_size))
            {
                lp_sde->lp_exception->error_callback++;
                lp_sde->status = False;
                return lp_sde;
            }
        }

        if (lp_dyna_array->elements_num > 1)
        {
            if (NULL_POINTER == memmove(((char *)lp_dyna_array->elements) + position * lp_dyna_array->element_size,
                                        (char *)lp_dyna_array->elements + (position + 1) * lp_dyna_array->element_size,
                                        (lp_dyna_array->elements_num - position - 1) * lp_dyna_array->element_size))
            {
                lp_sde->status = False;
                lp_sde->lp_exception->error_memove++;
                return lp_sde;
            }
        }

        if (NULL_POINTER == memset((char *)lp_dyna_array->elements + (lp_dyna_array->elements_num - 1) * lp_dyna_array->element_size,
                                   0, lp_dyna_array->element_size))
        {
            // TODO 恢复之前被移动的元素内存，或者抛出异常
            lp_sde->status = False;
            lp_sde->lp_exception->error_memset++;
            return lp_sde;
        }
    }
    else
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_dyna_array->elements_num--;
    return lp_sde;
}

/**
 * Gets element content based on element location
 *
 * params:
 *  LPDynaArray lp_dyna_array: dyna array pointer
 *  long position: the location where you want to get the element's content
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_index_out]
 */
LPStatusDataException DynaArray_get_by_position(LPDynaArray lp_dyna_array, long position)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return sde;

    if (lp_dyna_array == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (position >= 0 && position < lp_dyna_array->elements_num)
        lp_sde->data = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
    else
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
    }

    return lp_sde;
}

/**
 * Edit the element content based on the element position
 *
 * params:
 *  LPDynaArray lp_dyna_array: dyna array pointer
 *  long position: the location where you want to edit the element's content
 *  Object element: new element to be deposited
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_memcpy, error_index_out]
 */
LPStatusDataException DynaArray_edit_by_position(LPDynaArray lp_dyna_array, long position, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (position >= 0 && position < lp_dyna_array->elements_num)
    {
        if (NULL_POINTER == memcpy((char *)lp_dyna_array->elements + position * lp_dyna_array->element_size,
                                   element, lp_dyna_array->element_size))
        {
            lp_sde->status = False;
            lp_sde->lp_exception->error_memcpy++;
        }
        else
            lp_sde->data = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
    }
    else
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_index_out++;
    }

    return lp_sde;
}

/**
 * Edit elements based on their content
 *
 * params:
 *  LPDynaArray lp_dyna_array: dyna array pointer
 *  Object old_element: the element that was modified
 *  Object new_element: new element
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_malloc]
 */
LPStatusDataException DynaArray_edit_by_element(LPDynaArray lp_dyna_array, Object old_element, Object new_element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || old_element == NULL_POINTER || new_element == NULL_POINTER)
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_null_pointer++;
        return lp_sde;
    }

    int ls = sizeof(long);
    LPStatusDataException lp_sde_pos = DynaArray_new(10, ls);
    if (lp_sde_pos == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_sde_pos->status == False)
    {
        lp_sde->lp_exception->error_index_out += lp_sde_pos->lp_exception->error_index_out;
        lp_sde->lp_exception->error_malloc += lp_sde_pos->lp_exception->error_malloc;
        lp_sde->lp_exception->error_memset += lp_sde_pos->lp_exception->error_memset;
        StatusDataException_free(lp_sde_pos);
        return lp_sde;
    }

    LPDynaArray lp_da = (LPDynaArray)lp_sde_pos->data;
    StatusDataException_free(lp_sde_pos);

    LPStatusDataException lp_sde_iter;
    for (long i = 0; i < lp_dyna_array->elements_num; i++)
    {
        lp_sde_iter = DynaArray_get_by_position(lp_dyna_array, i);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde->lp_exception->error_malloc++;
            lp_sde->status = False;
            break;
        }
        if (lp_sde_iter->status == False)
        {
            lp_sde->lp_exception->error_null_pointer += lp_sde_iter->lp_exception->error_null_pointer;
            lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
            StatusDataException_free(lp_sde_iter);
            lp_sde->status = False;
            break;
        }
        if (memcmp((char *)lp_sde_iter->data, (char *)old_element, lp_dyna_array->element_size) == 0)
        {
            StatusDataException_free(lp_sde_iter);
            lp_sde_iter = DynaArray_edit_by_position(lp_dyna_array, i, new_element);
            if (lp_sde_iter == NULL_POINTER || lp_sde_iter->status == False)
            {

                lp_sde->lp_exception->error_null_pointer += lp_sde_iter->lp_exception->error_null_pointer;
                lp_sde->lp_exception->error_memcpy += lp_sde_iter->lp_exception->error_memcpy;
                lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
                lp_sde->status = False;
            }
            else
            {
                StatusDataException_free(lp_sde_iter);
                lp_sde_iter = DynaArray_insert(lp_da, lp_da->elements_num, &i);
                if (lp_sde_iter == NULL_POINTER || lp_sde_iter->status == False)
                {
                    lp_sde->lp_exception->error_null_pointer += lp_sde_iter->lp_exception->error_null_pointer;
                    lp_sde->lp_exception->error_malloc += lp_sde_iter->lp_exception->error_malloc;
                    lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
                    lp_sde->lp_exception->error_realloc += lp_sde_iter->lp_exception->error_realloc;
                    lp_sde->lp_exception->error_memset += lp_sde_iter->lp_exception->error_memset;
                    lp_sde->lp_exception->error_memove += lp_sde_iter->lp_exception->error_memove;
                    lp_sde->lp_exception->error_memcpy += lp_sde_iter->lp_exception->error_memcpy;
                    lp_sde->status = False;
                }
            }
        }
        StatusDataException_free(lp_sde_iter);
    }

    return lp_sde;
}

/**
 * Deletes elements in a dynamic array based on their contents
 *
 * params:
 *  LPDynaArray lp_dyna_array:
 *  Object old_element:
 *  Boolean (*func)(Object): callback function to delete the element's attribute memory
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_index_out, error_malloc, error_realloc,
 *              error_memset, error_callback, error_memove]
 */
LPStatusDataException DynaArray_delete_by_element(LPDynaArray lp_dyna_array, Object old_element, Boolean (*func)(Object))
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || old_element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_sde->data = lp_dyna_array;

    if (lp_dyna_array->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    LPStatusDataException lp_sde_iter;
    for (long i = 0; i < lp_dyna_array->elements_num; i++)
    {
        lp_sde_iter = DynaArray_get_by_position(lp_dyna_array, i);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde->lp_exception->error_null_pointer++;
            lp_sde->status = False;
            continue;
        }
        if (lp_sde_iter->status == False)
        {
            lp_sde->lp_exception->error_null_pointer += lp_sde_iter->lp_exception->error_null_pointer;
            lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
            lp_sde->status = False;
            StatusDataException_free(lp_sde_iter);
            continue;
        }
        if (memcmp((char *)lp_sde_iter->data, (char *)old_element, lp_dyna_array->element_size) == 0)
        {
            StatusDataException_free(lp_sde_iter);

            lp_sde_iter = DynaArray_delete_by_position(lp_dyna_array, i, func);
            if (lp_sde_iter == NULL_POINTER)
            {
                lp_sde->lp_exception->error_malloc++;
                lp_sde->status = False;
                continue;
            }
            if (lp_sde_iter->status == False)
            {
                lp_sde->lp_exception->error_malloc += lp_sde_iter->lp_exception->error_malloc;
                lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
                lp_sde->lp_exception->error_realloc += lp_sde_iter->lp_exception->error_realloc;
                lp_sde->lp_exception->error_memset += lp_sde_iter->lp_exception->error_memset;
                lp_sde->lp_exception->error_callback += lp_sde_iter->lp_exception->error_callback;
                lp_sde->lp_exception->error_memove += lp_sde_iter->lp_exception->error_memove;
                lp_sde->status = False;
                continue;
            }
            i--;
        }
        StatusDataException_free(lp_sde_iter);
    }

    return lp_sde;
}

/**
 * Gets element based on its position
 *
 * params:
 *  LPDynaArray lp_dyna_array: dyna array pointer
 *  Object element: search for the element
 *
 * return: LPStatusDataException that contains indexes dyna array
 *
 * exceptions: [error_null_pointer, error_index_out, error_malloc, error_memset,
 *              error_realloc, error_memove, error_memcpy]
 */
LPStatusDataException DynaArray_get_position_by_element(LPDynaArray lp_dyna_array, Object element)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER || element == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_dyna_array->elements_num == 0)
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    int long_size = sizeof(long);
    LPStatusDataException lp_sde_pos = DynaArray_new(10, long_size);
    if (lp_sde_pos == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        lp_sde->data = lp_dyna_array;
        return lp_sde;
    }
    if (lp_sde_pos->status == False)
    {
        lp_sde->lp_exception->error_index_out += lp_sde_pos->lp_exception->error_index_out;
        lp_sde->lp_exception->error_malloc += lp_sde_pos->lp_exception->error_malloc;
        lp_sde->lp_exception->error_memset += lp_sde_pos->lp_exception->error_memset;
        StatusDataException_free(lp_sde_pos);
        return lp_sde;
    }

    LPDynaArray lp_dn_l = (LPDynaArray)lp_sde_pos->data;
    StatusDataException_free(lp_sde_pos);

    LPStatusDataException lp_sde_iter;
    for (long i = 0; i < lp_dyna_array->elements_num; i++)
    {
        lp_sde_iter = DynaArray_get_by_position(lp_dyna_array, i);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde->lp_exception->error_null_pointer++;
            lp_sde->status = False;
            continue;
        }
        if (lp_sde_iter->status == False)
        {
            lp_sde->lp_exception->error_null_pointer += lp_sde_iter->lp_exception->error_null_pointer;
            lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
            lp_sde->status = False;
            StatusDataException_free(lp_sde_iter);
            continue;
        }
        if (memcmp((char *)lp_sde_iter->data, (char *)element, lp_dyna_array->element_size) == 0)
        {
            StatusDataException_free(lp_sde_iter);
            lp_sde_iter = DynaArray_insert(lp_dn_l, lp_dn_l->elements_num, &i);

            if (lp_sde_iter == NULL_POINTER)
            {
                lp_sde->lp_exception->error_null_pointer++;
                lp_sde->status = False;
                continue;
            }
            if (lp_sde_iter->status == False)
            {
                lp_sde->lp_exception->error_null_pointer += lp_sde_iter->lp_exception->error_null_pointer;
                lp_sde->lp_exception->error_malloc += lp_sde_iter->lp_exception->error_malloc;
                lp_sde->lp_exception->error_index_out += lp_sde_iter->lp_exception->error_index_out;
                lp_sde->lp_exception->error_realloc += lp_sde_iter->lp_exception->error_realloc;
                lp_sde->lp_exception->error_memset += lp_sde_iter->lp_exception->error_memset;
                lp_sde->lp_exception->error_memove += lp_sde_iter->lp_exception->error_memove;
                lp_sde->lp_exception->error_memcpy += lp_sde_iter->lp_exception->error_memcpy;
                lp_sde->status = False;
            }
        }
        StatusDataException_free(lp_sde_iter);
    }

    return lp_sde;
}
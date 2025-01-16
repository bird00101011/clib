// c dynamic array implementation
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
 *  Boolean (*copy_func)(Object, Object): a pointer to the memory replication function
 *  Boolean (*compare_func)(Object, Object): memory comparison function pointer
 *  Boolean (*free_func)(Object): pointer to the memory release function
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_index_out, error_malloc, error_memset]
 */
LPStatusDataException DynaArray_new(long capacity, long element_size,
                                    Boolean (*copy_func)(Object, Object),
                                    Boolean (*compare_func)(Object, Object),
                                    Boolean (*free_func)(Object))
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
    lpAl->copy_func = copy_func;
    lpAl->compare_func = compare_func;
    lpAl->free_func = free_func;

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
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_malloc, error_index_out, error_callback, error_null_pointer]
 */
LPStatusDataException DynaArray_free(LPDynaArray lp_dyna_array)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_dyna_array == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer++;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_dyna_array->elements != NULL_POINTER)
    {
        if (lp_dyna_array->free_func != NULL_POINTER)
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

                if (False == lp_dyna_array->free_func(lp_sde_iter->data))
                {
                    lp_sde->lp_exception->error_callback++;
                    lp_sde->status = False;
                }

                lp_dyna_array->elements_num--;
            }
        }

        free(lp_dyna_array->elements);
    }

    free(lp_dyna_array);

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
        chra *src = (char *)lp_dyna_array->elements + lp_dyna_array->capacity * lp_dyna_array->element_size;
        if (NULL_POINTER == memset(src, 0, new_mem_size - old_mem_size))
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

void _insert_kr(LPDynaArray lp_dyna_array, LPStatusDataException lp_sde, Boolean *reallocated)
{
    LPStatusDataException lp_sde_r = DynaArray_reallocate(lp_dyna_array, 2 * lp_dyna_array->capacity);
    if (lp_sde_r == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc++;
        lp_sde->status = False;
    }
    else
    {
        if (lp_sde_r->status == False)
        {
            lp_sde->lp_exception->error_malloc += lp_sde_r->lp_exception->error_malloc;
            lp_sde->lp_exception->error_index_out += lp_sde_r->lp_exception->error_index_out;
            lp_sde->lp_exception->error_realloc += lp_sde_r->lp_exception->error_realloc;
            lp_sde->lp_exception->error_memset += lp_sde_r->lp_exception->error_memset;
            lp_sde->status = False;
        }
        else
            *reallocated = True;
    }
    StatusDataException_free(lp_sde_r);
}

void _insert_no0_noelements_num(LPDynaArray lp_dyna_array, LPStatusDataException lp_sde, void *element, Boolean *reallocated)
{
    char *dst = (char *)lp_dyna_array->elements + (position + 1) * lp_dyna_array->element_size;
    char *src = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
    size_t count = (lp_dyna_array->elements_num - position) * lp_dyna_array->element_size;
    LPStatusDataException lp_sde_r;

    if (NULL_POINTER == memmove(dst, src, count))
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_memove++;
        if (*reallocated == True)
        {
            lp_sde_r = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
            if (lp_sde_r == NULL_POINTER)
                lp_sde->lp_exception->error_malloc++;
            else
            {
                if (lp_sde_r->status == False)
                {
                    lp_sde->lp_exception->error_malloc += lp_sde_r->lp_exception->error_malloc;
                    lp_sde->lp_exception->error_index_out += lp_sde_r->lp_exception->error_index_out;
                    lp_sde->lp_exception->error_realloc += lp_sde_r->lp_exception->error_realloc;
                    lp_sde->lp_exception->error_memset += lp_sde_r->lp_exception->error_memset;
                }
            }
            StatusDataException_free(lp_sde_r);
        }
    }
    else
    {
        if (NULL_POINTER == memcpy(src, (char *)element, lp_dyna_array->element_size))
        {
            lp_sde->status = False;
            lp_sde->lp_exception->error_memcpy++;

            if (NULL_POINTER == memmove(src, dst, count))
                lp_sde->lp_exception->error_memove++;

            if (*reallocated == True)
            {
                lp_sde_r = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
                if (lp_sde_r == NULL_POINTER)
                    lp_sde->lp_exception->error_malloc++;
                else
                {
                    if (lp_sde_r->status == False)
                    {
                        lp_sde->lp_exception->error_malloc += lp_sde_r->lp_exception->error_malloc;
                        lp_sde->lp_exception->error_index_out += lp_sde_r->lp_exception->error_index_out;
                        lp_sde->lp_exception->error_realloc += lp_sde_r->lp_exception->error_realloc;
                        lp_sde->lp_exception->error_memset += lp_sde_r->lp_exception->error_memset;
                    }
                }
                StatusDataException_free(lp_sde_r);
            }
        }
        else
        {
            if (lp_dyna_array->copy_func != NULL_POINTER)
            {
                if (lp_dyna_array->copy_func(src, element) == False)
                {
                    lp_sde->lp_exception->error_callback++;
                    lp_sde->status = False;

                    if (NULL_POINTER == memmove(src, dst, count))
                        lp_sde->lp_exception->error_memove++;

                    if (*reallocated == True)
                    {
                        lp_sde_r = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
                        if (lp_sde_r == NULL_POINTER)
                            lp_sde->lp_exception->error_malloc++;
                        else
                        {
                            if (lp_sde_r->status == False)
                            {
                                lp_sde->lp_exception->error_malloc += lp_sde_r->lp_exception->error_malloc;
                                lp_sde->lp_exception->error_index_out += lp_sde_r->lp_exception->error_index_out;
                                lp_sde->lp_exception->error_realloc += lp_sde_r->lp_exception->error_realloc;
                                lp_sde->lp_exception->error_memset += lp_sde_r->lp_exception->error_memset;
                            }
                        }
                        StatusDataException_free(lp_sde_r);
                    }
                }
            }
        }
    }
}

void _insert_elements_num(LPDynaArray lp_dyna_array, void *element, LPStatusDataException lp_sde, Boolean *reallocated)
{
    char *src = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
    LPStatusDataException lp_sde_r;

    if (NULL_POINTER == memcpy(src, (char *)element, lp_dyna_array->element_size))
    {
        lp_sde->status = False;
        lp_sde->lp_exception->error_memcpy++;

        if (*reallocated == True)
        {
            lp_sde_r = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
            if (lp_sde_r == NULL_POINTER)
                lp_sde->lp_exception->error_malloc++;
            else
            {
                if (lp_sde_r->status == False)
                {
                    lp_sde->lp_exception->error_malloc += lp_sde_r->lp_exception->error_malloc;
                    lp_sde->lp_exception->error_index_out += lp_sde_r->lp_exception->error_index_out;
                    lp_sde->lp_exception->error_realloc += lp_sde_r->lp_exception->error_realloc;
                    lp_sde->lp_exception->error_memset += lp_sde_r->lp_exception->error_memset;
                }
            }
            StatusDataException_free(lp_sde_r);
        }
    }
    if (lp_dyna_array->copy_func != NULL_POINTER)
    {
        if (lp_dyna_array->copy_func(src, element) == False)
        {
            lp_sde->lp_exception->error_callback++;
            lp_sde->status = False;

            if (*reallocated == True)
            {
                lp_sde_r = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
                if (lp_sde_r == NULL_POINTER)
                    lp_sde->lp_exception->error_malloc++;
                else
                {
                    if (lp_sde_r->status == False)
                    {
                        lp_sde->lp_exception->error_malloc += lp_sde_r->lp_exception->error_malloc;
                        lp_sde->lp_exception->error_index_out += lp_sde_r->lp_exception->error_index_out;
                        lp_sde->lp_exception->error_realloc += lp_sde_r->lp_exception->error_realloc;
                        lp_sde->lp_exception->error_memset += lp_sde_r->lp_exception->error_memset;
                    }
                }
                StatusDataException_free(lp_sde_r);
            }
        }
    }
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
 *              error_memove, error_memcpy, error_callback]
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
        Boolean *reallocated = (Boolean *)malloc(sizeof(Boolean));
        *reallocated = False;
        if (lp_dyna_array->elements_num == lp_dyna_array->capacity)
        {
            _insert_kr(lp_dyna_array, lp_sde, reallocated);
            free(reallocated);
            if (lp_sde->status == False)
                return lp_sde;
        }

        if (position < lp_dyna_array->elements_num)
            _insert_no0_noelements_num(lp_dyna_array, lp_sde, element, reallocated);
        else // position == elements_num
            _insert_elements_num(lp_dyna_array, element, lp_sde, reallocated);

        free(reallocated);
    }
    else
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
    }

    if (lp_sde->status == False)
        return lp_sde;

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
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_malloc, error_index_out, error_realloc, error_memset, error_callback,
 *              error_memove]
 */
LPStatusDataException DynaArray_delete_by_position(LPDynaArray lp_dyna_array, long position)
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

    if (position >= 0 && position < lp_dyna_array->elements_num)
    {
        if (lp_dyna_array->elements_num > 1)
        {
            char *dst = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
            char *src = (char *)lp_dyna_array->elements + (position + 1) * lp_dyna_array->element_size;
            size_t count = (lp_dyna_array->elements_num - position - 1) * lp_dyna_array->element_size;

            if (lp_dyna_array->free_func != NULL_POINTER)
            {
                if (False == lp_dyna_array->free_func(dst))
                {
                    lp_sde->lp_exception->error_callback++;
                    lp_sde->status = False;
                    return lp_sde;
                }
            }

            if (NULL_POINTER == memmove((dst, src, count))
            {
                // 移动失败，子属性内存难以恢复
                lp_sde->status = False;
                lp_sde->lp_exception->error_memove++;
                return lp_sde;
            }

            dst = (char *)lp_dyna_array->elements + (lp_dyna_array->elements_num - 1) * lp_dyna_array->element_size;
            if (NULL_POINTER == memset(dst, 0, lp_dyna_array->element_size))
            {
                // 设置末尾元素内存内容为0失败，其实也无大碍
                lp_sde->status = False;
                lp_sde->lp_exception->error_memset++;
                return lp_sde;
            }
        }
    }
    else
    {
        lp_sde->lp_exception->error_index_out++;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_dyna_array->elements_num--;

    if (lp_dyna_array->elements_num < lp_dyna_array->capacity / 2)
    {
        LPStatusDataException lp_sde_r = DynaArray_reallocate(lp_dyna_array, lp_dyna_array->capacity / 2);
        if (lp_sde_r == NULL_POINTER)
        {
            lp_sde->lp_exception->error_malloc++;
            lp_sde->status = False;
            return lp_sde;
        }
        if (lp_sde_r->status == False)
        {
            lp_sde->lp_exception->error_malloc += lp_sde_r->lp_exception->error_malloc;
            lp_sde->lp_exception->error_index_out += lp_sde_r->lp_exception->error_index_out;
            lp_sde->lp_exception->error_realloc += lp_sde_r->lp_exception->error_realloc;
            lp_sde->lp_exception->error_memset += lp_sde_r->lp_exception->error_memset;
            StatusDataException_free(lp_sde_r);
            lp_sde->status = False;
        }
    }

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
        return lp_sde;

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
        char *src = (char *)lp_dyna_array->elements + position * lp_dyna_array->element_size;
        if (lp_dyna_array->copy_func != NULL_POINTER)
        {
            if (False == lp_dyna_array->copy_func(src, element))
            {
                lp_sde->lp_exception->error_callback++;
                lp_sde->status = False;
            }
            else
                lp_sde->data = src;
        }
        else
        {
            if (NULL_POINTER == memcpy(src, (char *)element, lp_dyna_array->element_size))
            {
                lp_sde->status = False;
                lp_sde->lp_exception->error_memcpy++;
            }
            else
                lp_sde->data = src;
        }
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
        lp_sde->status = False;
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

        StatusDataException_free(lp_sde_iter);
        Boolean equ = False;

        if (lp_dyna_array->compare_func != NULL_POINTER)
            equ = lp_dyna_array->compare_func((char *)lp_sde_iter->data, (char *)old_element);
        else
        {
            if (memcmp((char *)lp_sde_iter->data, (char *)old_element, lp_dyna_array->element_size) == 0)
                equ = True;
        }

        if (equ == True)
        {
            lp_sde_iter = DynaArray_edit_by_position(lp_dyna_array, i, new_element);
            if (lp_sde_iter == NULL_POINTER)
            {
                lp_sde->lp_exception->error_malloc++;
                lp_sde->status = False;
            }
            else
            {
                if (lp_sde_iter->status == False)
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
                    if (lp_sde_iter == NULL_POINTER)
                    {
                        lp_sde->lp_exception->error_malloc++;
                        lp_sde->status = False;
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
                    StatusDataException_free(lp_sde_iter);
                }
            }
        }
    }

    lp_sde->data = lp_da;
    return lp_sde;
}

/**
 * Deletes elements in a dynamic array based on their contents
 *
 * params:
 *  LPDynaArray lp_dyna_array:
 *  Object old_element:
 *  Boolean (*free_func)(Object): callback function to delete the element's attribute memory
 *
 * return: LPStatusDataException
 *
 * exceptions: [error_null_pointer, error_index_out, error_malloc, error_realloc,
 *              error_memset, error_callback, error_memove]
 */
LPStatusDataException DynaArray_delete_by_element(LPDynaArray lp_dyna_array, Object old_element, Boolean (*free_func)(Object))
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

        void *data = lp_sde_iter->data;
        StatusDataException_free(lp_sde_iter);
        Boolean equ = False;

        if (lp_dyna_array->compare_func != NULL_POINTER)
            equ = lp_dyna_array->compare_func(data, old_element);
        else
        {
            if (memcmp((char *)lp_sde_iter->data, (char *)old_element, lp_dyna_array->element_size) == 0)
                equ = True;
        }

        if (equ == True)
        {
            lp_sde_iter = DynaArray_delete_by_position(lp_dyna_array, i);
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
            }
            else
                i--;

            StatusDataException_free(lp_sde_iter);
        }
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
        Boolean equ = False;
        void *data = lp_sde_iter->data;
        StatusDataException_free(lp_sde_iter);

        if (lp_dyna_array->compare_func != NULL_POINTER)
            equ = lp_dyna_array->compare_func(data, element);
        else
        {
            if (memcmp((char *)data, (char *)element, lp_dyna_array->element_size) == 0)
                equ = True;
        }

        if (equ == True)
        {
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
            StatusDataException_free(lp_sde_iter);
        }
    }

    lp_sde->data = lp_dn_l;
    return lp_sde;
}
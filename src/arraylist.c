#include <stdlib.h>
#include <string.h>
#include <arraylist.h>

LPStatusDataException ArrayList_new(long capacity, long element_size)
{
    LPStatusDataException lde = StatusDataException_new();
    if (lde == NULL_POINTER)
        return lde;

    if (capacity <= 0 || element_size <= 0)
    {
        lde->lp_exception->error_index_out = True;
        lde->status = False;
        return lde;
    }

    LPArrayList lpAl = malloc(sizeof(ArrayList));
    // 若分配失败返回NULL_POINTER指针
    if (lpAl == NULL_POINTER)
    {
        lde->status = False;
        lde->lp_exception->error_malloc = True;
        return lde;
    }
    lde->data = lpAl;

    lpAl->elements_num = 0;
    lpAl->element_size = element_size;
    lpAl->iter_index = 0;

    // 若元素个数小于5，将分配5个元素大小的实际容量
    lpAl->capacity = capacity > 5 ? capacity : 5;

    // 为数组分配内存
    lpAl->elements = malloc(lpAl->capacity * lpAl->element_size);
    // 分配失败，则返回NULL_POINTER指针，同时释放分配的lpAl内存，并且避免lpAl野指针，由于前面已经分配成功lpAl
    if (lpAl->elements == NULL_POINTER)
    {
        free(lpAl);

        lde->status = False;
        lde->lp_exception->error_malloc = True;

        return lde;
    }
    // 所有成员初始化为0，如果初始化失败，则返回NULL_POINTER指针，并释放分配的数组和列表内存
    if (NULL_POINTER == memset(lpAl->elements, 0, lpAl->capacity * lpAl->element_size))
    {
        free(lpAl->elements);
        free(lpAl);
        lde->status = False;
        lde->lp_exception->error_memset = True;

        return lde;
    }

    return lde;
}

LPStatusDataException ArrayList_free(LPArrayList lp_arraylist)
{
    LPStatusDataException lde = StatusDataException_new();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist != NULL_POINTER)
    {
        if (lp_arraylist->elements != NULL_POINTER)
        {
            free(lp_arraylist->elements);
            lp_arraylist->elements = NULL_POINTER; // 防止野指针
        }

        free(lp_arraylist);
    }
    else
    {
        lde->lp_exception->error_null_pointer = True;
        lde->status = False;
        lde->data = lp_arraylist;
    }

    return lde;
}

LPStatusDataException ArrayList_reallocate(LPArrayList lp_arraylist, long new_capacity)
{
    LPStatusDataException lde = StatusDataException_new();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER)
    {
        lde->lp_exception->error_null_pointer = True;
        lde->status = False;
        return lde;
    }
    lde->data = lp_arraylist;

    if (new_capacity <= 0)
    {
        lde->lp_exception->error_index_out = True;
        lde->status = False;

        return lde;
    }

    // 获取新容量的内存大小
    long new_mem_size = new_capacity * lp_arraylist->element_size;

    // 获取老容量的内存大小
    long old_mem_size = lp_arraylist->capacity * lp_arraylist->element_size;

    // 为保存数据的数组分配新的大小的内存空间
    void *new_elements = realloc(lp_arraylist->elements, new_mem_size);

    if (new_elements == NULL_POINTER)
    {
        lde->data = lp_arraylist;
        lde->lp_exception->error_realloc = True;
        lde->status = False;

        return lde;
    }

    // 使列表数组指针指向新的
    lp_arraylist->elements = new_elements;

    // 如果是增加容量，则将扩展的容量的元素内存内容设置为0
    // 如果是减少容量，则不设置
    // 此时，列表原容量还没有改变
    if (lp_arraylist->capacity < new_capacity)
    {
        // 如果设置内存失败
        if (NULL_POINTER == memset((char *)lp_arraylist->elements + lp_arraylist->capacity * lp_arraylist->element_size,
                                   0, new_mem_size - old_mem_size))
        {
            // 恢复扩展的内存到开始大小
            new_elements = realloc(lp_arraylist->elements, lp_arraylist->capacity);
            if (new_elements == NULL_POINTER)
            {
                lde->lp_exception->error_realloc = True;
                lde->status = False;

                return lde;
            }

            lde->lp_exception->error_memset = True;
            lde->status = False;

            return lde;
        }
    }

    lp_arraylist->capacity = new_capacity;
    // 如何列表存储的元素个数大于新的容量，则列表存储的元素个数等于新的容量
    // TODO 那这里有个问题，被遗弃的数组元素内存没有释放，变成野内存了
    // 如果列表存储的元素 个数小于新的容量，则列表存储的元素个数不变
    if (lp_arraylist->elements_num > lp_arraylist->capacity)
    {
        lp_arraylist->elements_num = lp_arraylist->capacity;
    }

    return lde;
}

LPStatusDataException ArrayList_insert(LPArrayList lp_arraylist, long position, Object element)
{
    LPStatusDataException lde = StatusDataException_new();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER || element == NULL_POINTER)
    {
        lde->lp_exception->error_null_pointer = True;
        lde->status = False;
        return lde;
    }

    lde->data = lp_arraylist;

    if (position >= 0 && position <= lp_arraylist->elements_num)
    {
        // 如果元素个数即将超过容量
        char sig = 0;
        if (lp_arraylist->elements_num == lp_arraylist->capacity)
        {
            sig = 1;
            // 分配2倍大小的容量
            LPStatusDataException ar_lde = ArrayList_reallocate(lp_arraylist, 2 * lp_arraylist->capacity);
            if (ar_lde->status != True)
            {
                StatusDataException_free(lde);
                return ar_lde;
            }

            StatusDataException_free(ar_lde);
        }

        if (position < lp_arraylist->elements_num)
        {
            // 将插入点后面的数据全部向右偏移元素字节数
            if (NULL_POINTER == memmove(((char *)lp_arraylist->elements) + (position + 1) * lp_arraylist->element_size,
                                        (char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                        (lp_arraylist->elements_num - position) * lp_arraylist->element_size))
            {
                lde->status = False;
                lde->lp_exception->error_memove = True;

                return lde;
            }

            // 将元素所指向的内存复制到插入节点上
            if (NULL_POINTER == memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                       element, lp_arraylist->element_size))
            {
                // TODO 恢复之前被移动的元素内存，或者抛出异常
                lde->status = False;
                lde->lp_exception->error_memcpy = True;

                return lde;
            }
        }
        else // position == elements_num
        {
            // 将元素所指向的内存复制到数组末端
            if (NULL_POINTER == memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                       element, lp_arraylist->element_size))
            {
                if (sig == 1)
                {
                    LPStatusDataException ar_lde = ArrayList_reallocate(lp_arraylist, lp_arraylist->capacity / 2);
                    if (ar_lde->status != True)
                    {
                        // 这里打印日志或者返回其它值，如果只返回这一个值，那复制内存失败的信息就不能返回
                        StatusDataException_free(lde);

                        return ar_lde;
                    }

                    StatusDataException_free(ar_lde);
                }
                else
                {
                    lde->lp_exception->error_memcpy = True;
                    lde->status = False;
                    return lde;
                }
            }
        }
    }
    else
    {
        lde->lp_exception->error_index_out = True;
        lde->status = False;

        return lde;
    }

    // 然后将实际元素个数+1
    lp_arraylist->elements_num = lp_arraylist->elements_num + 1; // 也可以用++
    return lde;
}

LPStatusDataException ArrayList_delete_element_by_position(LPArrayList lp_arraylist, long position)
{
    LPStatusDataException lde = StatusDataException_new();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER)
    {
        lde->lp_exception->error_null_pointer = True;
        lde->status = False;

        return lde;
    }

    lde->data = lp_arraylist;

    if (lp_arraylist->elements_num == 0)
    {
        lde->lp_exception->error_index_out = True;
        lde->status = False;

        return lde;
    }

    if (lp_arraylist->elements_num < lp_arraylist->capacity / 2)
    {
        LPStatusDataException rsde = ArrayList_reallocate(lp_arraylist, lp_arraylist->capacity / 2);
        if (rsde->status == False)
        {
            StatusDataException_free(lde);
            return rsde;
        }
    }

    if (position >= 0 && position < lp_arraylist->elements_num)
    {
        if (lp_arraylist->elements_num > 1)
        {
            // 将删除点后面的数据全部向左偏移元素字节数
            if (NULL_POINTER == memmove(((char *)lp_arraylist->elements) + position * lp_arraylist->element_size,
                                        (char *)lp_arraylist->elements + (position + 1) * lp_arraylist->element_size,
                                        (lp_arraylist->elements_num - position - 1) * lp_arraylist->element_size))
            {
                lde->status = False;
                lde->lp_exception->error_memove = True;

                return lde;
            }
        }

        // 将末尾移动后的位置所指向的内存设置为0
        if (NULL_POINTER == memset((char *)lp_arraylist->elements + (lp_arraylist->elements_num - 1) * lp_arraylist->element_size,
                                   0, lp_arraylist->element_size))
        {
            // TODO 恢复之前被移动的元素内存，或者抛出异常
            lde->status = False;
            lde->lp_exception->error_memset = True;

            return lde;
        }
    }
    else
    {
        lde->lp_exception->error_index_out = True;
        lde->status = False;

        return lde;
    }

    lp_arraylist->elements_num--;
    lde->status = True;

    return lde;
}

LPStatusDataException ArrayList_iter(LPArrayList lp_arraylist)
{
    LPStatusDataException sde = StatusDataException_new();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist == NULL_POINTER)
    {
        sde->status = False;
        sde->lp_exception->error_null_pointer = True;
        return sde;
    }

    if (lp_arraylist->iter_index >= lp_arraylist->elements_num)
    {
        sde->lp_exception->error_iter_stop = True;
        lp_arraylist->iter_index = 0;
        sde->status = False;
    }
    else
    {
        sde->data = (char *)lp_arraylist->elements + lp_arraylist->iter_index * lp_arraylist->element_size;
        lp_arraylist->iter_index++;
    }

    return sde;
}

LPStatusDataException ArrayList_iter_stop(LPArrayList lp_arraylist)
{
    LPStatusDataException sde = StatusDataException_new();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist == NULL_POINTER)
    {
        sde->lp_exception->error_null_pointer = True;
        sde->status = False;
    }
    else
    {
        lp_arraylist->iter_index = 0;
        sde->data = lp_arraylist;
    }
    return sde;
}

LPStatusDataException ArrayList_get_element_by_position(LPArrayList lp_arraylist, long position)
{
    LPStatusDataException sde = StatusDataException_new();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist == NULL_POINTER)
    {
        sde->lp_exception->error_null_pointer = True;
        sde->status = False;
        return sde;
    }

    sde->data = lp_arraylist;

    if (position >= 0 && position < lp_arraylist->elements_num)
    {
        sde->data = (char *)lp_arraylist->elements + position * lp_arraylist->element_size;
    }
    else
    {
        sde->lp_exception->error_index_out = True;
        sde->status = False;
    }

    return sde;
}

LPStatusDataException ArrayList_edit_element_by_position(LPArrayList lp_arraylist, long position, Object element)
{
    LPStatusDataException sde = StatusDataException_new();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist == NULL_POINTER || element == NULL_POINTER)
    {
        sde->lp_exception->error_null_pointer = True;
        sde->status = False;

        return sde;
    }

    sde->data = lp_arraylist;

    if (position >= 0 && position < lp_arraylist->elements_num)
    {
        // 将元素所指向的内存复制到插入节点上
        if (NULL_POINTER == memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                   element, lp_arraylist->element_size))
        {
            sde->status = False;
            sde->lp_exception->error_memcpy = True;
        }
        else
        {
            sde->data = lp_arraylist;
        }
    }
    else
    {
        sde->status = False;
        sde->lp_exception->error_index_out = True;
    }

    return sde;
}

LPStatusDataException ArrayList_edit_element_by_element(LPArrayList lp_arraylist, Object old_element, Object new_element)
{
    LPStatusDataException sde = StatusDataException_new();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist == NULL_POINTER || old_element == NULL_POINTER || new_element == NULL_POINTER)
    {
        sde->status = False;
        sde->lp_exception->error_null_pointer = True;
        return sde;
    }

    LPStatusDataException isde = ArrayList_iter_stop(lp_arraylist);
    if (isde == NULL_POINTER || isde->status == False)
    {
        StatusDataException_free(sde);
        return isde;
    }
    StatusDataException_free(isde);
    isde = ArrayList_iter(lp_arraylist);
    sde->data = lp_arraylist;

    while (isde != NULL_POINTER && isde->lp_exception->error_iter_stop != True)
    {
        if (memcmp((char *)isde->data, (char *)old_element, lp_arraylist->element_size) == 0)
        {
            if (memcpy((char *)isde->data, (char *)new_element, lp_arraylist->element_size) == NULL_POINTER)
            {
                sde->status = False;
                sde->lp_exception->error_memcpy = True;
            }
        }

        StatusDataException_free(isde);
        isde = ArrayList_iter(lp_arraylist);
    }

    StatusDataException_free(isde);

    return sde;
}

LPStatusDataException ArrayList_delete_element_by_element(LPArrayList lp_arraylist, Object old_element)
{
    LPStatusDataException lde = StatusDataException_new();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER || old_element == NULL_POINTER)
    {
        lde->lp_exception->error_null_pointer = True;
        lde->status = False;
        lde->data = NULL_POINTER;

        return lde;
    }

    lde->data = lp_arraylist;

    if (lp_arraylist->elements_num == 0)
    {
        lde->lp_exception->error_index_out = True;
        lde->status = False;

        return lde;
    }

    LPStatusDataException isde = ArrayList_iter_stop(lp_arraylist);
    if (isde == NULL_POINTER || isde->status == False)
    {
        StatusDataException_free(lde);
        return isde;
    }
    StatusDataException_free(isde);

    isde = ArrayList_iter(lp_arraylist);
    LPStatusDataException dsde;

    while (isde != NULL_POINTER && isde->lp_exception->error_iter_stop != True)
    {
        if (memcmp((char *)isde->data, (char *)old_element, lp_arraylist->element_size) == 0)
        {
            dsde = ArrayList_delete_element_by_position(lp_arraylist, lp_arraylist->iter_index - 1);
            if (dsde == NULL_POINTER || dsde->status == False)
            {
                StatusDataException_free(isde);
                StatusDataException_free(lde);
                return dsde;
            }
            StatusDataException_free(dsde);
        }

        StatusDataException_free(isde);
        isde = ArrayList_iter(lp_arraylist);
    }

    StatusDataException_free(isde);

    lde->data = lp_arraylist;
    lde->status = True;

    return lde;
}

LPStatusDataException ArrayList_get_position_by_element(LPArrayList lp_arraylist, Object element)
{
    LPStatusDataException lde = StatusDataException_new();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER || element == NULL_POINTER)
    {
        lde->lp_exception->error_null_pointer = True;
        lde->status = False;
        lde->data = NULL_POINTER;

        return lde;
    }

    lde->data = lp_arraylist;

    if (lp_arraylist->elements_num == 0)
    {
        lde->lp_exception->error_index_out = True;
        lde->status = False;

        return lde;
    }

    LPStatusDataException isde = ArrayList_iter_stop(lp_arraylist);
    if (isde == NULL_POINTER || isde->status == False)
    {
        StatusDataException_free(lde);
        return isde;
    }
    StatusDataException_free(isde);

    isde = ArrayList_iter(lp_arraylist);
    Boolean sig = False;
    while (isde != NULL_POINTER && isde->lp_exception->error_iter_stop != True)
    {
        StatusDataException_free(isde);

        if (memcmp((char *)isde->data, (char *)element, lp_arraylist->element_size) == 0)
        {
            sig = True;
            break;
        }

        isde = ArrayList_iter(lp_arraylist);
    }

    if (sig)
    {
        char long_size = sizeof(long);
        lde->data = malloc(long_size);
        char pos = (lp_arraylist->iter_index - 1);
        if (NULL_POINTER == memcpy((char *)lde->data, &pos, long_size))
        {
            lde->lp_exception->error_memcpy = True;
            lde->status = False;
        }
    }
    else
        lde->status = False;

    return lde;
}
#include <stdlib.h>
#include <string.h>

#if defined(WINDOWS)
#include <windows.h>
#endif

#include <types.h>
#include <arraylist.h>

StatusDataError *arraylist_new(long capacity, long element_size)
{
    StatusDataError *lde = init_status_data_error();
    if (lde == NULL_POINTER)
        return lde;

    ArrayList *lpAl = malloc(sizeof(ArrayList));
    // 若分配失败返回NULL_POINTER指针
    if (lpAl == NULL_POINTER)
    {
        lde->status = NOTOK;
        lde->error->error_malloc = YES;
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
        lpAl = NULL_POINTER;

        lde->status = NOTOK;
        lde->error->error_malloc = YES;
        lde->data = NULL_POINTER;

        return lde;
    }
    // 所有成员初始化为0，如果初始化失败，则返回NULL_POINTER指针，并释放分配的数组和列表内存
    if (NULL_POINTER == memset(lpAl->elements, 0, lpAl->capacity * lpAl->element_size))
    {
        free(lpAl->elements);
        lpAl->elements = NULL_POINTER;
        free(lpAl);
        lpAl = NULL_POINTER;

        lde->data = NULL_POINTER;
        lde->status = NOTOK;
        lde->error->error_memset = YES;

        return lde;
    }

    return lde;
}

StatusDataError *arraylist_free(ArrayList *lp_arraylist)
{
    StatusDataError *lde = init_status_data_error();
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

        lde->status = OK;
        lde->data = NULL_POINTER;
    }
    else
    {
        lde->error->error_null_pointer = YES;
        lde->status = NOTOK;
        lde->data = NULL_POINTER;
    }

    return lde;
}

StatusDataError *arraylist_reallocate(ArrayList *lp_arraylist, long new_capacity)
{
    StatusDataError *lde = init_status_data_error();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER)
    {
        lde->error->error_null_pointer = YES;
        lde->status = NOTOK;
        return lde;
    }

    if (new_capacity <= 0)
    {
        lde->error->error_index_out = YES;
        lde->status = NOTOK;

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
        lde->error->error_realloc = YES;
        lde->status = NOTOK;

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
        if (NULL_POINTER == memset((char *)lp_arraylist->elements + (lp_arraylist->capacity - 1) * lp_arraylist->element_size,
                                   0, new_mem_size - old_mem_size))
        {
            // 恢复扩展的内存到开始大小
            new_elements = realloc(lp_arraylist->elements, lp_arraylist->capacity);
            if (new_elements == NULL_POINTER)
            {
                lde->error->error_realloc = YES;
                lde->status = NOTOK;

                return lde;
            }

            lde->error->error_memset = YES;
            lde->status = NOTOK;

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

    lde->data = lp_arraylist;
    lde->status = OK;

    return lde;
}

StatusDataError *arraylist_insert(ArrayList *lp_arraylist, long position, void *element)
{
    StatusDataError *lde = init_status_data_error();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER)
    {
        lde->error->error_null_pointer = YES;
        lde->status = NOTOK;
        return lde;
    }

    if (position >= 0 && position <= lp_arraylist->elements_num)
    {
        // 如果元素个数即将超过容量
        if (lp_arraylist->elements_num == lp_arraylist->capacity)
        {
            // 分配2倍大小的容量
            StatusDataError *ar_lde = arraylist_reallocate(lp_arraylist, 2 * lp_arraylist->capacity);
            if (ar_lde->status != OK)
            {
                free_status_data_error(lde);

                return ar_lde;
            }

            free_status_data_error(ar_lde);
        }

        if (position < lp_arraylist->elements_num)
        {
            // 将插入点后面的数据全部向右偏移元素字节数
            if (NULL_POINTER == memmove(((char *)lp_arraylist->elements) + (position + 1) * lp_arraylist->element_size,
                                        (char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                        (lp_arraylist->elements_num - position - 1) * lp_arraylist->element_size))
            {
                lde->status = NOTOK;
                lde->error->error_memove = YES;

                return lde;
            }

            // 将元素所指向的内存复制到插入节点上
            if (NULL_POINTER == memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                       element, lp_arraylist->element_size))
            {
                // TODO 恢复之前被移动的元素内存，或者抛出异常
                lde->status = NOTOK;
                lde->error->error_memcpy = YES;

                return lde;
            }
        }
        else // 等于 position = elements_num
        {
            // 将元素所指向的内存复制到数组末端
            if (NULL_POINTER == memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                       element, lp_arraylist->element_size))
            {
                StatusDataError *ar_lde = arraylist_reallocate(lp_arraylist, lp_arraylist->capacity / 2);
                if (ar_lde->status != OK)
                {
                    // 这里打印日志或者返回其它值，如果只返回这一个值，那复制内存失败的信息就不能返回
                    free_status_data_error(lde);

                    return ar_lde;
                }

                free_status_data_error(ar_lde);
            }
        }
    }
    else
    {
        lde->error->error_index_out = YES;
        lde->status = NOTOK;
        lde->data = NULL_POINTER;

        return lde;
    }

    // 然后将实际元素个数+1
    lp_arraylist->elements_num = lp_arraylist->elements_num + 1; // 也可以用++

    lde->data = lp_arraylist;
    lde->status = OK;

    return lde;
}

StatusDataError *arraylist_delete_element_by_position(ArrayList *lp_arraylist, long position)
{
    StatusDataError *lde = init_status_data_error();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER)
    {
        lde->error->error_null_pointer = YES;
        lde->status = NOTOK;
        lde->data = NULL_POINTER;

        return lde;
    }

    if (lp_arraylist->elements_num == 0)
    {
        lde->error->error_index_out = YES;
        lde->status = NOTOK;

        return lde;
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
                lde->status = NOTOK;
                lde->error->error_memove = YES;

                return lde;
            }
        }

        // 将末尾移动后的位置所指向的内存设置为0
        if (NULL_POINTER == memset((char *)lp_arraylist->elements + (lp_arraylist->elements_num - 1) * lp_arraylist->element_size,
                                   0, lp_arraylist->element_size))
        {
            // TODO 恢复之前被移动的元素内存，或者抛出异常
            lde->status = NOTOK;
            lde->error->error_memset = YES;

            return lde;
        }
    }
    else
    {
        lde->error->error_index_out = YES;
        lde->status = NOTOK;
        lde->data = NULL_POINTER;

        return lde;
    }

    lp_arraylist->elements_num--;
    lde->data = lp_arraylist;
    lde->status = OK;

    return lde;
}

StatusDataError *arraylist_iter(ArrayList *lp_arraylist)
{
    StatusDataError *sde = init_status_data_error();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist->iter_index >= lp_arraylist->elements_num)
    {
        sde->data = NULL_POINTER;
        sde->error->error_iter_stop = YES;
        lp_arraylist->iter_index = 0;
    }
    else
    {
        sde->data = (char *)lp_arraylist->elements + lp_arraylist->iter_index * lp_arraylist->element_size;
        lp_arraylist->iter_index++;
    }

    sde->status = OK;

    return sde;
}

StatusDataError *arraylist_stop_iter(ArrayList *lp_arraylist)
{
    StatusDataError *sde = init_status_data_error();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist == NULL_POINTER)
    {
        sde->error->error_null_pointer = YES;
        sde->status = NOTOK;
    }
    else
    {
        lp_arraylist->iter_index = 0;
        sde->status = OK;
    }
    return sde;
}

StatusDataError *arraylist_get_element_by_position(ArrayList *al, long position)
{
    StatusDataError *sde = init_status_data_error();
    if (sde == NULL_POINTER)
        return sde;

    if (al == NULL_POINTER)
    {
        sde->error->error_null_pointer = YES;
        return sde;
    }

    if (position >= 0 && position < al->elements_num)
    {
        sde->data = (char *)al->elements + position * al->element_size;
        sde->status = OK;
    }
    else
    {
        sde->error->error_index_out = YES;
        sde->status = NOTOK;
    }

    return sde;
}

StatusDataError *arraylist_edit_element_by_position(ArrayList *lp_arraylist, long position, void *element)
{
    StatusDataError *sde = init_status_data_error();
    if (sde == NULL_POINTER)
        return sde;

    if (lp_arraylist == NULL_POINTER || element == NULL_POINTER)
    {
        sde->error->error_null_pointer = YES;
        sde->data = NULL_POINTER;
        sde->status = NOTOK;

        return sde;
    }

    if (position >= 0 && position < lp_arraylist->elements_num)
    {
        // 将元素所指向的内存复制到插入节点上
        if (NULL_POINTER == memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                                   element, lp_arraylist->element_size))
        {
            sde->status = NOTOK;
            sde->error->error_memcpy = YES;
        }
        else
        {
            sde->status = OK;
            sde->data = lp_arraylist;
        }
    }
    else
    {
        sde->status = NOTOK;
        sde->error->error_index_out = YES;
    }

    return sde;
}

StatusDataError *arraylist_edit_element_by_element(ArrayList *lp_arraylist, void *old_element, void *new_element)
{
    StatusDataError *sde = init_status_data_error();
    if (sde == NULL_POINTER)
        return sde;

    if (old_element == NULL_POINTER || new_element == NULL_POINTER)
    {
        sde->status = NOTOK;
        sde->error->error_null_pointer = YES;
        return sde;
    }

    StatusDataError *isde = arraylist_stop_iter(lp_arraylist);
    if (isde == NULL_POINTER || isde->status == NOTOK)
    {
        free_status_data_error(sde);
        return isde;
    }
    free_status_data_error(isde);
    isde = arraylist_iter(lp_arraylist);

    while (isde != NULL_POINTER && isde->error->error_iter_stop != YES)
    {
        if (memcmp((char *)isde->data, (char *)old_element, lp_arraylist->element_size) == 0)
        {
            if (memcpy((char *)isde->data, (char *)new_element, lp_arraylist->element_size) == NULL_POINTER)
            {
                sde->status = NOTOK;
                sde->error->error_memcpy = YES;
            }
            else
                sde->status = OK;
        }

        free_status_data_error(isde);

        isde = arraylist_iter(lp_arraylist);
    }

    free_status_data_error(isde);

    return sde;
}

StatusDataError *arraylist_delete_element_by_element(ArrayList *lp_arraylist, void *old_element)
{
    StatusDataError *lde = init_status_data_error();
    if (lde == NULL_POINTER)
        return lde;

    if (lp_arraylist == NULL_POINTER || old_element == NULL_POINTER)
    {
        lde->error->error_null_pointer = YES;
        lde->status = NOTOK;
        lde->data = NULL_POINTER;

        return lde;
    }

    if (lp_arraylist->elements_num == 0)
    {
        lde->error->error_index_out = YES;
        lde->status = NOTOK;

        return lde;
    }

    StatusDataError *isde = arraylist_stop_iter(lp_arraylist);
    if (isde == NULL_POINTER || isde->status == NOTOK)
    {
        free_status_data_error(lde);
        return isde;
    }
    free_status_data_error(isde);

    isde = arraylist_iter(lp_arraylist);
    StatusDataError *dsde;

    while (isde != NULL_POINTER && isde->error->error_iter_stop != YES)
    {
        if (memcmp((char *)isde->data, (char *)old_element, lp_arraylist->element_size) == 0)
        {
            dsde = arraylist_delete_element_by_position(lp_arraylist, lp_arraylist->iter_index - 1);
            if (dsde == NULL_POINTER || dsde->status == NOTOK)
            {
                free_status_data_error(isde);
                free_status_data_error(lde);
                return dsde;
            }
            free_status_data_error(dsde);
        }

        free_status_data_error(isde);
        isde = arraylist_iter(lp_arraylist);
    }

    free_status_data_error(isde);

    lde->data = lp_arraylist;
    lde->status = OK;

    return lde;
}

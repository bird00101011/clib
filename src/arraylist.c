#include <stdlib.h>
#if defined(WINDOWS)
#include <windows.h>
#endif

#include <types.h>
#include <arraylist.h>

StatusDataError *arraylist_new(long elements_num, long element_size)
{
    StatusDataError *lde = malloc(sizeof(StatusDataError));
    CLIBError *error = malloc(sizeof(CLIBError));
    lde->error = error;

    ArrayList *lpAl = malloc(sizeof(ArrayList));
    // 若分配失败返回NULL_POINTER指针
    if (lpAl == NULL_POINTER)
    {
        lde->data = NULL_POINTER;
        lde->status = NOTOK;
        lde->error->error_malloc = YES;
        return lde;
    }

    lpAl->elements_num = elements_num;
    lpAl->element_size = element_size;

    // 若元素个数小于5，将分配5个元素大小的实际容量
    lpAl->capacity = elements_num > 5 ? elements_num : 5;

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
    if (0 != memset(lpAl->elements, 0, lpAl->capacity * lpAl->element_size))
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
    StatusDataError *lde = malloc(sizeof(StatusDataError));
    CLIBError *error = malloc(sizeof(CLIBError));
    lde->error = error;

    if (lp_arraylist != NULL_POINTER)
    {
        if (lp_arraylist->elements != NULL_POINTER)
        {
            free(lp_arraylist->elements);
            lp_arraylist->elements = NULL_POINTER; // 防止野指针
        }

        free(lp_arraylist);
        lp_arraylist = NULL_POINTER; // 防止野指针

        lde->status = OK;
        lde->data = NULL_POINTER;
    }
    else
    {
        lde->error->error_not_necessarily_operate = YES;
    }

    return lde;
}

StatusDataError *arraylist_reallocate(ArrayList *lp_arraylist, long new_capacity)
{
    StatusDataError *lde = malloc(sizeof(StatusDataError));
    CLIBError *error = malloc(sizeof(CLIBError));
    lde->error = error;

    // 获取新容量的内存大小
    long new_mem_size = new_capacity * lp_arraylist->element_size;

    // 获取老容量的内存大小
    long old_mem_size = lp_arraylist->capacity * lp_arraylist->element_size;

    // 为保存数据的数组分配新的大小的内存空间
    void *new_elements = realloc(lp_arraylist->elements, new_mem_size);

    // 分配成功
    if (new_elements != NULL_POINTER)
    {
        // 使列表数组指针指向新的
        lp_arraylist->elements = new_elements;

        // 如果是增加容量，则将扩展的容量的元素内存内容设置为0
        // 如果是减少容量，则不设置
        // 此时，列表原容量还没有改变
        if (lp_arraylist->capacity < new_capacity)
        {
            // 如果设置内存失败
            if (0 != memset(((char *)lp_arraylist->elements) + lp_arraylist->capacity, 0, new_mem_size - old_mem_size))
            {
                lde->data = lp_arraylist;
                lde->error->error_memset = YES;
                lde->status = NOTOK;
                return lde;
            }
        }
    }
    else
    {
        lde->data = lp_arraylist;
        lde->error->error_realloc = YES;
        lde->status = NOTOK;

        return lde;
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
    StatusDataError *lde = malloc(sizeof(StatusDataError));
    CLIBError *error = malloc(sizeof(CLIBError));
    lde->error = error;

    // PS. 内存是从0 开始的，所以，插入末端的时候，你看pos=数组实际元素个数的时候不是pos等于索引
    //     所以，插入的末端的时候，不需要移动，都没数据移动啥呢？
    // 如果插入位置大于数组实际存储元素个数，则插入失败
    if (position > lp_arraylist->elements_num)
    {
        lde->status = NOTOK;
        lde->data = lp_arraylist;

        return lde;
    }

    else if (position < lp_arraylist->elements_num)
    {
        // 将插入点后面的数据全部向右偏移元素字节数
        if (0 != memmove(((char *)lp_arraylist->elements) + (position + 1) * lp_arraylist->element_size,
                         (char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                         (lp_arraylist->elements_num - position) * lp_arraylist->element_size))
        {
            lde->status = NOTOK;
            lde->data = lp_arraylist;
            lde->error->error_memove = YES;

            return lde;
        }

        // 将元素所指向的内存复制到插入节点上
        if (0 != memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                        element, lp_arraylist->element_size))
        {
            // TODO 恢复之前被移动的元素内存，火车抛出异常
            lde->status = NOTOK;
            lde->data = lp_arraylist;
            lde->error->error_memcpy = YES;

            return lde;
        }
    }
    else
    {
        int rc;
        // 如果元素个数即将超过容量
        if (lp_arraylist->elements_num = lp_arraylist->capacity)
        {
            // 分配2倍大小的容量
            StatusDataError *ar_lde = arraylist_reallocate(lp_arraylist, 2 * lp_arraylist->capacity);
            if (ar_lde->status != OK)
            {
                free(error);
                free(lde);
                error = NULL_POINTER;
                lde = NULL_POINTER;

                return ar_lde;
            }
            else
            {
                free(ar_lde->error);
                ar_lde->error = NULL_POINTER;

                free(ar_lde);
                ar_lde = NULL_POINTER;
            }
        }

        // 将元素所指向的内存复制到数组末端
        if (0 != memcpy((char *)lp_arraylist->elements + position * lp_arraylist->element_size,
                        element, lp_arraylist->element_size))
        {
            StatusDataError *ar_lde = arraylist_reallocate(lp_arraylist, lp_arraylist->capacity / 2);
            // 复制都失败了，恢复刚才扩容的内存
            if (ar_lde->status != OK)
            {
                // 这里打印日志或者返回其它值，如果只返回这一个值，那复制内存失败的信息就不能返回
                free(error);
                free(lde);

                error = NULL_POINTER;
                lde = NULL_POINTER;

                return ar_lde;
            }
            else
            {
                free(ar_lde->error);
                ar_lde->error = NULL_POINTER;

                free(ar_lde);
                ar_lde = NULL_POINTER;
            }
        }
    }

    // 然后将实际元素个数+1
    lp_arraylist->elements_num = lp_arraylist->elements_num + 1; // 也可以用++

    lde->data = lp_arraylist;
    lde->status = OK;

    return lde;
}
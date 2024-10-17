#include <stdlib.h>
#if defined(WINDOWS)
#include <windows.h>
#endif
#include <arraylist.h>

// 初始化一个ArrayList
// 成功返回ArrayList指针，失败返回NULL指针
LPArrayList arraylist_new(long elements_num, long element_size)
{
    LPArrayList lpAl = malloc(sizeof(ArrayList));
    // 若分配失败返回NULL指针
    if (lpAl == NULL)
    {
        return NULL;
    }

    lpAl->elements_num = elements_num;
    lpAl->element_size = element_size;

    // 若元素个数小于5，将分配5个元素大小的实际容量
    lpAl->capacity = elements_num > 5 ? elements_num : 5;

    // 为数组分配内存
    lpAl->elements = malloc(lpAl->capacity * lpAl->element_size);
    // 分配失败，则返回NULL指针，同时释放分配的lpAl内存，并且避免lpAl野指针，由于前面已经分配成功lpAl
    if (lpAl->elements == NULL)
    {
        free(lpAl);
        lpAl = NULL;

        return NULL;
    }
    // 所有成员初始化为0，如果初始化失败，则返回NULL指针，并释放分配的数组和列表内存
    if (0 != memset(lpAl->elements, 0, lpAl->capacity * lpAl->element_size))
    {
        free(lpAl->elements);
        lpAl->elements = NULL;
        free(lpAl);
        lpAl = NULL;

        return NULL;
    }

    return lpAl;
}

// 释放一个ArrayList
// 如果释放失败，就是操作系统内核负责的事情了
void arraylist_free(LPArrayList lp_arraylist)
{
    if (lp_arraylist != NULL)
    {
        if (lp_arraylist->elements != NULL)
        {
            free(lp_arraylist->elements);
            lp_arraylist->elements = NULL; // 防止野指针
        }

        free(lp_arraylist);
        lp_arraylist = NULL; // 防止野指针
    }
}

// 为数组扩（增加或减小）容
// 成功返回TRUE，失败返回FALSE
int arraylist_reallocate(LPArrayList lp_arraylist, long new_capacity)
{
    // 获取新容量的内存大小
    long new_mem_size = new_capacity * lp_arraylist->element_size;

    // 获取老容量的内存大小
    long old_mem_size = lp_arraylist->capacity * lp_arraylist->element_size;

    // 为保存数据的数组分配新的大小的内存空间
    void *new_elements = realloc(lp_arraylist->elements, new_mem_size);

    // 分配成功
    if (new_elements != NULL)
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
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }

    lp_arraylist->capacity = new_capacity;
    // 如何列表存储的元素个数大于新的容量，则列表存储的元素个数等于新的容量
    // 如果列表存储的元素 个数小于新的容量，则列表存储的元素个数不变
    if (lp_arraylist->elements_num > lp_arraylist->capacity)
    {
        lp_arraylist->elements_num = lp_arraylist->capacity;
    }

    return TRUE;
}

// 在指定位置后插入元素
int arraylist_insert(LPArrayList lp_arraylist, long position, void *element);

// 删除指定位置的元素
int arraylist_delete_element_by_position(LPArrayList lp_arraylist, long position);

// 删除指定元素内容的元素
int arraylist_delete_element_by_element(LPArrayList lp_arraylist, void *element);

// 编辑指定位置的元素
int arraylist_edit_element_by_position(LPArrayList lp_arraylist, long position, void *element);

// 编辑指定元素内容的元素
int arraylist_edit_element_by_element(LPArrayList lp_arraylist, int *old_element, int *new_element);

// 获取指定位置的元素
void *arraylist_get_element_by_position(LPArrayList lp_arraylist, long position);

// 获取指定元素的位置
long arraylist_get_position_by_element(LPArrayList lp_arraylist, void *element);

// 右边追加元素
int arraylist_lappend(LPArrayList lp_arraylist, void *element);

// 左边追加元素
int arraylist_rappend(LPArrayList lp_arraylist, void *element);
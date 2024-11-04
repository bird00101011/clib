#include <types.h>
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#if defined(WINDOWS)
#include <windows.h>
#endif

typedef struct
{
    void *elements;    // 保存数据的数组
    long elements_num; // 列表存储的元素个数
    long element_size; // 单个元素字节数
    long capacity;     // 列表容量
    long iter_index;   // 迭代器的位置
#if defined(WINDOWS)
    HANDLE g_mutex; // Windows下的互斥锁
#endif
} ArrayList;

// 初始化一个ArrayList
StatusDataError *arraylist_new(long capacity, long element_size);

// 释放一个ArrayList
StatusDataError *arraylist_free(ArrayList *lp_arraylist);

// 为数组扩容
StatusDataError *arraylist_reallocate(ArrayList *lp_arraylist, long new_capacity);

// 在指定位置后插入元素
StatusDataError *arraylist_insert(ArrayList *lp_arraylist, long position, void *element);

// 删除指定位置的元素
StatusDataError *arraylist_delete_element_by_position(ArrayList *lp_arraylist, long position);

// 迭代元素
StatusDataError *arraylist_iter(ArrayList *lp_arraylist);

// 停止迭代
StatusDataError *arraylist_stop_iter(ArrayList *lp_arraylist);

// 删除指定元素内容的元素
StatusDataError *arraylist_delete_element_by_element(ArrayList *lp_arraylist, void *element);

// 编辑指定位置的元素
StatusDataError *arraylist_edit_element_by_position(ArrayList *lp_arraylist, long position, void *element);

// 编辑指定元素内容的元素
StatusDataError *arraylist_edit_element_by_element(ArrayList *lp_arraylist, void *old_element, void *new_element);

// 获取指定位置的元素
StatusDataError *arraylist_get_element_by_position(ArrayList *lp_arraylist, long position);

// 获取指定元素的位置
StatusDataError *arraylist_get_position_by_element(ArrayList *lp_arraylist, void *element);

#endif
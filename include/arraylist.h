#include <types.h>
#ifndef ArrayList_H
#define ArrayList_H

#if defined(WINDOWS)
#include <windows.h>
#endif

typedef struct
{
    Object elements;   // 保存数据的数组
    long elements_num; // 列表存储的元素个数
    long element_size; // 单个元素字节数
    long capacity;     // 列表容量
    long iter_index;   // 迭代器的位置
#if defined(WINDOWS)
    HANDLE g_mutex; // Windows下的互斥锁
#endif
} ArrayList, *LPArrayList;

// 初始化一个ArrayList
LPStatusDataException ArrayList_new(long capacity, long element_size);

// 释放一个ArrayList
LPStatusDataException ArrayList_free(LPArrayList lp_arraylist);

// 为数组扩容
LPStatusDataException ArrayList_reallocate(LPArrayList lp_arraylist, long new_capacity);

// 在指定位置后插入元素
LPStatusDataException ArrayList_insert(LPArrayList lp_arraylist, long position, Object element);

// 删除指定位置的元素
LPStatusDataException ArrayList_delete_element_by_position(LPArrayList lp_arraylist, long position);

// 迭代元素
LPStatusDataException ArrayList_iter(LPArrayList lp_arraylist);

// 停止迭代
LPStatusDataException ArrayList_iter_stop(LPArrayList lp_arraylist);

// 删除指定元素内容的元素
LPStatusDataException ArrayList_delete_element_by_element(LPArrayList lp_arraylist, Object element);

// 编辑指定位置的元素
LPStatusDataException ArrayList_edit_element_by_position(LPArrayList lp_arraylist, long position, Object element);

// 编辑指定元素内容的元素
LPStatusDataException ArrayList_edit_element_by_element(LPArrayList lp_arraylist, Object old_element, Object new_element);

// 获取指定位置的元素
LPStatusDataException ArrayList_get_element_by_position(LPArrayList lp_arraylist, long position);

// 获取指定元素的位置
LPStatusDataException ArrayList_get_position_by_element(LPArrayList lp_arraylist, Object element);

#endif
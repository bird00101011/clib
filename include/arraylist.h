#if defined(WINDOWS)
#include <windows.h>
#endif

// 定义TRUE, FALSE< NULL指针
#ifndef TRUE
#define TRUE 0
#endif

#ifndef FALSE
#define FALSE -1
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

// 列表结构体
typedef struct
{
    void *elements;    // 保存数据的数组
    long elements_num; // 列表存储的元素个数
    long element_size; // 单个元素字节数
    long capacity;     // 列表容量
#if defined(WINDOWS)
    HANDLE g_mutex; // Windows下的互斥锁
#endif
} ArrayList, *LPArrayList;

// 初始化一个ArrayList
LPArrayList arraylist_new(long elements_num, long element_size);

// 释放一个ArrayList
void arraylist_free(LPArrayList lp_arraylist);

// 为数组扩容
int arraylist_reallocate(LPArrayList lp_arraylist, long new_capacity);

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
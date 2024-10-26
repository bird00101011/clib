#include <types.h>

// 初始化一个ArrayList
StatusDataError *arraylist_new(long elements_num, long element_size);

// 释放一个ArrayList
StatusDataError *arraylist_free(ArrayList *lp_arraylist);

// 为数组扩容
StatusDataError *arraylist_reallocate(ArrayList *lp_arraylist, long new_capacity);

// 在指定位置后插入元素
StatusDataError *arraylist_insert(ArrayList *lp_arraylist, long position, void *element);

// 删除指定位置的元素
StatusDataError *arraylist_delete_element_by_position(ArrayList *lp_arraylist, long position);

// 删除指定元素内容的元素
StatusDataError *arraylist_delete_element_by_element(ArrayList *lp_arraylist, void *element);

// 编辑指定位置的元素
StatusDataError *arraylist_edit_element_by_position(ArrayList *lp_arraylist, long position, void *element);

// 编辑指定元素内容的元素
StatusDataError *arraylist_edit_element_by_element(ArrayList *lp_arraylist, int *old_element, int *new_element);

// 获取指定位置的元素
StatusDataError *arraylist_get_element_by_position(ArrayList *lp_arraylist, long position);

// 获取指定元素的位置
StatusDataError *arraylist_get_position_by_element(ArrayList *lp_arraylist, void *element);

// 右边追加元素
StatusDataError *arraylist_lappend(ArrayList *lp_arraylist, void *element);

// 左边追加元素
StatusDataError *arraylist_rappend(ArrayList *lp_arraylist, void *element);
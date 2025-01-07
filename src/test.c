#include <types.h>
#include <arraylist.h>
#include <stdio.h>
/*
printf格式化选项
%d 十进制有符号整数
%u 十进制无符号整数
%f 浮点数
%s 字符串
%c 单个字符
%p 指针的值
%e 指数形式的浮点数
%x, %X 无符号以十六进制表示的整数
%o 无符号以八进制表示的整数
%g 把输出的值按照 %e 或者 %f 类型中输出长度较小的方式输出
%p 输出地址符
%lu 32位无符号整数
%llu 64位无符号整数
%% 输出百分号字符本身。
*/

void test_arraylist();

int main()
{
    test_arraylist();
    return 0;
}

void test_arraylist()
{
    LPStatusDataException lde = ArrayList_new(10, sizeof(int));
    if (lde == NULL_POINTER)
        return;

    if (lde->status == False)
    {
        StatusDataException_free(lde);
        printf("[E] ArrayList_new\n");
        return;
    }

    ArrayList *al = (ArrayList *)(lde->data);
    if (al == NULL_POINTER)
    {
        StatusDataException_free(lde);
        printf("[E] ArrayList_new\n");
        return;
    }

    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    int n1 = 1;
    int n2 = 2;
    int n3 = 3;
    int n4 = 4;
    int n5 = 5;
    int n6 = 6;
    int n7 = 7;
    int n8 = 8;

    StatusDataException_free(lde);
    lde = NULL_POINTER;
    lde = ArrayList_insert(al, 0, &n1);
    if (lde == NULL_POINTER)
    {
        ArrayList_free(al);
        StatusDataException_free(lde);

        printf("[E] ArrayList_insert\n");
        return;
    }

    if (lde->status == False)
    {
        StatusDataException_free(lde);
        ArrayList_free(al);

        printf("[E] ArrayList_insert\n");
        return;
    }

    StatusDataException_free(lde);

    lde = ArrayList_insert(al, 1, &n2);
    StatusDataException_free(lde);

    lde = ArrayList_insert(al, 2, &n3);
    StatusDataException_free(lde);

    lde = ArrayList_insert(al, 3, &n4);
    StatusDataException_free(lde);

    lde = ArrayList_insert(al, 4, &n5);
    StatusDataException_free(lde);

    lde = ArrayList_insert(al, 5, &n6);
    StatusDataException_free(lde);

    lde = ArrayList_insert(al, 6, &n7);
    StatusDataException_free(lde);

    lde = ArrayList_insert(al, 7, &n8);
    StatusDataException_free(lde);

    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    lde = ArrayList_iter(al);
    int *element;
    while (lde != NULL_POINTER && lde->lp_exception->error_iter_stop != True)
    {
        element = (int *)(lde->data);
        printf("1 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        StatusDataException_free(lde);
        lde = ArrayList_iter(al);
    }
    // ArrayList_iter_stop(al);

    StatusDataException_free(lde);

    lde = ArrayList_delete_element_by_position(al, 0);
    if (lde == NULL_POINTER)
    {
        ArrayList_free(al);
        StatusDataException_free(lde);
        lde = NULL_POINTER;

        printf("[E] arraylist_delete_element_by_position\n");
        return;
    }

    printf("After deleted al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    if (lde->status == False)
    {
        StatusDataException_free(lde);
        ArrayList_free(al);

        printf("[E] arraylist_delete_element_by_position\n");
        return;
    }
    StatusDataException_free(lde);

    lde = ArrayList_iter(al);
    while (lde != NULL_POINTER && lde->lp_exception->error_iter_stop == False)
    {
        element = (int *)(lde->data);
        printf("2 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        StatusDataException_free(lde);

        lde = ArrayList_iter(al);
    }

    lde = ArrayList_get_element_by_position(al, 8);
    if (lde == NULL_POINTER)
    {
        ArrayList_free(al);

        printf("[E] arraylist_get_element_by_position\n");
        return;
    }
    element = (int *)lde->data;
    if (lde->lp_exception->error_index_out == False)
        printf("al 1 element: %d\n", *element);
    else
        printf("al 8 element: index out\n");

    StatusDataException_free(lde);

    printf("After edit by position\n");
    int n = 100;
    lde = ArrayList_edit_element_by_position(al, 1, &n);
    StatusDataException_free(lde);
    n = 101;

    lde = ArrayList_iter(al);
    while (lde != NULL_POINTER && lde->lp_exception->error_iter_stop == False)
    {
        element = (int *)(lde->data);
        printf("2 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        StatusDataException_free(lde);
        lde = ArrayList_iter(al);
    }

    lde = ArrayList_edit_element_by_element(al, &n8, &n);
    printf("After edit by element, elements_num: %d\n", al->elements_num);

    StatusDataException_free(lde);

    lde = ArrayList_iter(al);
    while (lde != NULL_POINTER && lde->lp_exception->error_iter_stop == False)
    {
        element = (int *)(lde->data);
        printf("3 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        StatusDataException_free(lde);
        lde = ArrayList_iter(al);
    }

    lde = ArrayList_delete_element_by_element(al, &n);
    printf("After del by element, elements_num: %d\n", al->elements_num);

    StatusDataException_free(lde);

    lde = ArrayList_iter(al);
    while (lde != NULL_POINTER && lde->lp_exception->error_iter_stop == False)
    {
        element = (int *)(lde->data);
        printf("4 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        StatusDataException_free(lde);
        lde = ArrayList_iter(al);
    }

    // 扩容测试
    int ns[100];
    for (int i = 0; i < 100; i++)
    {
        ns[i] = i;
        lde = ArrayList_insert(al, al->elements_num, &ns[i]);
        StatusDataException_free(lde);
    }

    printf("After add 10 elements, elements_num: %d, %p\n", al->elements_num, lde->lp_exception);

    lde = ArrayList_iter(al);
    LPStatusDataException nde;
    while (lde != NULL_POINTER && lde->lp_exception->error_iter_stop == False)
    {
        element = (int *)(lde->data);
        nde = ArrayList_get_element_by_position(al, al->iter_index - 1);
        printf("4 %d, %p, iter: %d, %d, %d\n", al->iter_index - 1, element, *element, *(int *)(nde->data), *((int *)al->elements + al->iter_index - 1));
        StatusDataException_free(lde);
        lde = ArrayList_iter(al);
    }

    lde = ArrayList_get_position_by_element(al, &n1);
    printf("n1 pos: %lu\n", *(long *)(lde->data));

    StatusDataException_free(lde);
    ArrayList_free(al);
    al = NULL_POINTER;
    lde = NULL_POINTER;
}
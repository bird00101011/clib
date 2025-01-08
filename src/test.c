#include <types.h>
#include <dynaarray.h>
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
    LPStatusDataException lde = DynaArray_new(10, sizeof(int));
    if (lde == NULL_POINTER)
        return;

    if (lde->status == False)
    {
        StatusDataException_free(lde);
        printf("[E] DynaArray_new\n");
        return;
    }

    DynaArray *al = (DynaArray *)(lde->data);
    if (al == NULL_POINTER)
    {
        StatusDataException_free(lde);
        printf("[E] DynaArray_new\n");
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
    lde = DynaArray_insert(al, 0, &n1);
    if (lde == NULL_POINTER)
    {
        DynaArray_free(al);
        StatusDataException_free(lde);

        printf("[E] DynaArray_insert\n");
        return;
    }

    if (lde->status == False)
    {
        StatusDataException_free(lde);
        DynaArray_free(al);

        printf("[E] DynaArray_insert\n");
        return;
    }

    StatusDataException_free(lde);

    lde = DynaArray_insert(al, 1, &n2);
    StatusDataException_free(lde);

    lde = DynaArray_insert(al, 2, &n3);
    StatusDataException_free(lde);

    lde = DynaArray_insert(al, 3, &n4);
    StatusDataException_free(lde);

    lde = DynaArray_insert(al, 4, &n5);
    StatusDataException_free(lde);

    lde = DynaArray_insert(al, 5, &n6);
    StatusDataException_free(lde);

    lde = DynaArray_insert(al, 6, &n7);
    StatusDataException_free(lde);

    lde = DynaArray_insert(al, 7, &n8);
    StatusDataException_free(lde);

    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    int *element;
    for (long index = 0; index < al->elements_num; index++)
    {
        lde = DynaArray_get_by_position(al, index);
        element = (int *)(lde->data);
        printf("1 %d, %p, iter: %d\n", index, element, *element);
        StatusDataException_free(lde);
    }
    // DynaArray_iter_stop(al);

    StatusDataException_free(lde);

    lde = DynaArray_delete_by_position(al, 0);
    if (lde == NULL_POINTER)
    {
        DynaArray_free(al);
        StatusDataException_free(lde);
        lde = NULL_POINTER;

        printf("[E] arraylist_delete_element_by_position\n");
        return;
    }

    printf("After deleted al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    if (lde->status == False)
    {
        StatusDataException_free(lde);
        DynaArray_free(al);

        printf("[E] arraylist_delete_element_by_position\n");
        return;
    }
    StatusDataException_free(lde);
    for (long index = 0; index < al->elements_num; index++)
    {
        lde = DynaArray_get_by_position(al, index);
        element = (int *)(lde->data);
        printf("2 %d, %p, iter: %d\n", index, element, *element);
        StatusDataException_free(lde);
    }

    lde = DynaArray_get_by_position(al, 8);
    if (lde == NULL_POINTER)
    {
        DynaArray_free(al);

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
    lde = DynaArray_edit_by_position(al, 1, &n);
    StatusDataException_free(lde);
    n = 101;

    for (long index = 0; index < al->elements_num; index++)
    {
        lde = DynaArray_get_by_position(al, index);
        element = (int *)(lde->data);
        printf("2 %d, %p, iter: %d\n", index, element, *element);
    }

    lde = DynaArray_edit_by_element(al, &n8, &n);
    printf("After edit by element, elements_num: %d\n", al->elements_num);

    StatusDataException_free(lde);

    for (long index = 0; index < al->elements_num; index++)
    {
        lde = DynaArray_get_by_position(al, index);
        element = (int *)(lde->data);
        printf("3 %d, %p, iter: %d\n", index, element, *element);
    }

    lde = DynaArray_delete_by_element(al, &n);
    printf("After del by element, elements_num: %d\n", al->elements_num);

    StatusDataException_free(lde);
    for (long index = 0; index < al->elements_num; index++)
    {
        lde = DynaArray_get_by_position(al, index);
        element = (int *)(lde->data);
        printf("4 %d, %p, iter: %d\n", index, element, *element);
    }

    // 扩容测试
    int ns[100];
    for (int i = 0; i < 100; i++)
    {
        ns[i] = i;
        lde = DynaArray_insert(al, al->elements_num, &ns[i]);
        StatusDataException_free(lde);
    }

    printf("After add 10 elements, elements_num: %d, %p\n", al->elements_num, lde->lp_exception);

    StatusDataException_free(lde);
    for (long index = 0; index < al->elements_num; index++)
    {
        lde = DynaArray_get_by_position(al, index);
        element = (int *)(lde->data);
        printf("4 %d, %p, iter: %d, %d\n", index, element, *element,  *((int *)al->elements + index));
        StatusDataException_free(lde);
    }

    lde = DynaArray_get_position_by_element(al, &n1);
    printf("n1 pos: %lu\n", *(long *)(lde->data));

    StatusDataException_free(lde);
    DynaArray_free(al);
    al = NULL_POINTER;
    lde = NULL_POINTER;
}
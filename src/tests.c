#include <types.h>
#include <dynaarray.h>
#include <linkedlist.h>
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

void test_dnayarray();
void test_linkedlist();

int main()
{
    test_linkedlist();
    // test_dnayarray();
    return 0;
}

void test_linkedlist()
{
    printf("LinkedList Tests......................................\n");
    LPStatusDataException lp_sde = LinkedList_new(sizeof(int));
    if (lp_sde == NULL_POINTER || lp_sde->status == False)
    {
        printf("LinkedList_new(10): failed\n");
    }
    printf("LinkedList_new(10): success\n");

    LPLinkedList lp_ll = (LPLinkedList)lp_sde->data;
    StatusDataException_free(lp_sde);

    int n, n1, n2, n3, n4, n5, n6, n7, n8, n9;
    n = 0;
    n1 = 1;
    n2 = 2;
    n3 = 3;
    n4 = 4;
    n5 = 5;
    n6 = 6;
    n7 = 7;
    n8 = 8;
    n9 = 9;
    StatusDataException_free(LinkedList_insert(lp_ll, &n, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n1, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n2, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n3, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n4, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n5, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n6, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n7, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n8, lp_ll->elements_num));
    StatusDataException_free(LinkedList_insert(lp_ll, &n9, lp_ll->elements_num));
    printf("After inserted 3 '9'\n");
    StatusDataException_free(LinkedList_insert(lp_ll, &n9, 0));
    lp_sde = LinkedList_insert(lp_ll, &n9, 1);
    if (lp_sde->status == False)
        printf("LinkedList_insert(lp_ll, &n9, 1): failed\n");
    else
        printf("LinkedList_insert(lp_ll, &n9, 1): success\n");

    StatusDataException_free(LinkedList_insert(lp_ll, &n9, 3));

    printf("n=0,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8,n9=9\n");
    printf("Begin traverse LinkedList instance after inserted 10 numbers in [0..9]:\n");
    for (long i = 0; i < lp_ll->elements_num; i++)
    {
        lp_sde = LinkedList_get_by_position(lp_ll, i);
        printf("index=%d,element_addr=%p,element_content=%d\n", i, lp_sde->data, *(int *)lp_sde->data);
        StatusDataException_free(lp_sde);
    }
    printf("End traverse\n");
    LinkedList_free(lp_ll);
}

void test_dnayarray()
{
    printf("DynaArray Tests.......................................\n");
    LPStatusDataException lde = DynaArray_new(10, sizeof(int));
    if (lde == NULL_POINTER)
    {
        printf("DynaArray_new(10, sizeof(int)): faield(return NULL_POINTER)\n");
    }
    else if (lde->status == False)
    {
        StatusDataException_free(lde);
        printf("DynaArray_new(10, sizeof(int)): failed(return status=False)\n");
    }
    else
    {
        printf("DynaArray_new(10, sizeof(int)): success\n");
        LPDynaArray al = (LPDynaArray)(lde->data);

        int n1 = 1;
        int n2 = 2;
        int n3 = 3;
        int n4 = 4;
        int n5 = 5;
        int n6 = 6;
        int n7 = 7;
        int n8 = 8;

        printf("n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8\n");
        LPStatusDataException ldei = DynaArray_insert(al, 0, &n1);
        if (lde == NULL_POINTER)
            printf("DynaArray_insert(al, 0, &n1): failed\n");
        else
        {
            if (lde->status == False)
                printf("DynaArray_insert(al, 0, &n1)(al, 0, &n1): failed\n");
            else
                printf("DynaArray_insert(al, 0, &n1): success\n");
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
        printf("Now DynaArray instance's elements_num=%d, capacity=%d\n", al->elements_num, al->capacity);

        printf("Begin traverse DynaArray instance:\n");
        int *element;
        for (long index = 0; index < al->elements_num; index++)
        {
            lde = DynaArray_get_by_position(al, index);
            element = (int *)(lde->data);
            printf("\tindex=%d, element_addr=%p, element_content=%d\n", index, element, *element);
            StatusDataException_free(lde);
        }
        printf("End traverse DynaArray instance.\n");

        lde = DynaArray_delete_by_position(al, 0);
        if (lde == NULL_POINTER)
            printf("DynaArray_delete_by_position(al, 0): failed\n");
        else
        {
            if (lde->status == False)
                printf("DynaArray_delete_by_position(al, 0): failed\n");
            else
                printf("DynaArray_delete_by_position(al, 0): success\n");
        }
        printf("Now DynaArray instance's elements_num=%d, capacity=%d\n", al->elements_num, al->capacity);
        printf("Begin traverse DynaArray instance:\n");
        StatusDataException_free(lde);
        for (long index = 0; index < al->elements_num; index++)
        {
            lde = DynaArray_get_by_position(al, index);
            element = (int *)(lde->data);
            printf("\tindex=%d, element_addr=%p, element_content=%d\n", index, element, *element);
            StatusDataException_free(lde);
        }
        printf("End traverse DynaArray instance.\n");

        lde = DynaArray_get_by_position(al, 8);
        if (lde == NULL_POINTER)
            printf("DynaArray_get_by_position(al, 8): failed(return NULL_POINTER)\n");

        element = (int *)lde->data;
        if (lde->lp_exception->error_index_out == True)
            printf("DynaArray index out: success\n");
        else
            printf("DynaArray index out: failed\n");

        StatusDataException_free(lde);
        printf("n=100,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8\n");
        int n = 100;
        lde = DynaArray_edit_by_position(al, 1, &n);
        *(int *)lde->data = 101;
        StatusDataException_free(lde);
        printf("n=101,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8\n");
        if (lde == NULL_POINTER)
            printf("DynaArray_edit_by_position(al, 1, &n)\n");
        else
            printf("DynaArray_edit_by_position(al, 1, &n): success\n");

        printf("Now DynaArray instance's elements_num=%d, capacity=%d\n", al->elements_num, al->capacity);
        printf("Begin traverse DynaArray instance:\n");
        for (long index = 0; index < al->elements_num; index++)
        {
            lde = DynaArray_get_by_position(al, index);
            element = (int *)(lde->data);
            printf("\tindex=%d, element_addr=%p, element_content=%d\n", index, element, *element);
            StatusDataException_free(lde);
        }
        printf("End traverse DynaArray instance.\n");

        lde = DynaArray_edit_by_element(al, &n8, &n);
        if (lde == NULL_POINTER)
            printf("DynaArray_edit_by_element(al, &n8, &n): faield\n");
        else
            printf("DynaArray_edit_by_element(al, &n8, &n): success\n");

        StatusDataException_free(lde);
        printf("Now DynaArray instance's elements_num=%d, capacity=%d\n", al->elements_num, al->capacity);
        printf("n=101,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8\n");
        printf("Begin traverse DynaArray instance:\n");
        for (long index = 0; index < al->elements_num; index++)
        {
            lde = DynaArray_get_by_position(al, index);
            element = (int *)(lde->data);
            printf("\tindex=%d, element_addr=%p, element_content=%d\n", index, element, *element);
            StatusDataException_free(lde);
        }
        printf("End traverse DynaArray instance.\n");

        lde = DynaArray_delete_by_element(al, &n);
        if (lde == NULL_POINTER)
            printf("DynaArray_delete_by_element(al, &n): faield\n");
        else
            printf("DynaArray_delete_by_element(al, &n): success\n");

        printf("Now DynaArray instance's elements_num=%d, capacity=%d\n", al->elements_num, al->capacity);
        printf("n=101,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8\n");
        printf("Begin traverse DynaArray instance:\n");
        StatusDataException_free(lde);
        for (long index = 0; index < al->elements_num; index++)
        {
            lde = DynaArray_get_by_position(al, index);
            element = (int *)(lde->data);
            printf("\tindex=%d, element_addr=%p, element_content=%d\n", index, element, *element);
            StatusDataException_free(lde);
        }
        printf("End traverse DynaArray instance.\n");

        // 扩容测试
        printf("DynaArray_inert 100 elements [0..99]:\n");
        int ns[100];
        for (int i = 0; i < 100; i++)
        {
            ns[i] = i;
            lde = DynaArray_insert(al, al->elements_num, &ns[i]);
            StatusDataException_free(lde);
        }

        printf("Now DynaArray instance's elements_num=%d, capacity=%d\n", al->elements_num, al->capacity);
        printf("n=101,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8\n");
        printf("Begin traverse DynaArray instance:\n");
        for (long index = 0; index < al->elements_num; index++)
        {
            lde = DynaArray_get_by_position(al, index);
            element = (int *)(lde->data);
            printf("\tindex=%d, element_addr=%p, element_content=%d, element_content=%d\n", index, element, *element, *((int *)al->elements + index));
            StatusDataException_free(lde);
        }
        printf("End traverse DynaArray instance.\n");

        int n9 = 1;
        StatusDataException_free(DynaArray_insert(al, al->elements_num, &n9));

        lde = DynaArray_get_position_by_element(al, &n1);
        if (lde == NULL_POINTER)
            printf("DynaArray_get_position_by_element(al, &n1): failed\n");
        else
        {
            if (lde->status == False)
                printf("DynaArray_get_position_by_element(al, &n1): failed\n");
            else
            {
                printf("DynaArray_get_position_by_element(al, &n1): success\n");
                printf("n=101,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8,n9=1\n");
                printf("Begin traverse index of DynaArray_get_position_by_element(al, &n1):\n");
                LPDynaArray rows = (LPDynaArray)lde->data;
                StatusDataException_free(lde);
                for (long index = 0; index < rows->elements_num; index++)
                {
                    lde = DynaArray_get_by_position(rows, index);
                    printf("\tindex=%d, element_addr=%p, element_content=%d, element_content=%d\n", index, (long *)lde->data, *(long *)lde->data, *((long *)rows->elements + index));
                    StatusDataException_free(lde);
                }
                printf("End traverse DynaArray instance.\n");
            }
        }
        int o = 1000;
        lde = DynaArray_edit_by_element(al, &n1, &o);
        if (lde == NULL_POINTER)
            printf("DynaArray_edit_by_element(al, &n1): failed\n");
        else
        {
            if (lde->status == False)
                printf("DynaArray_get_position_by_element(al, &n1): failed\n");
            else
            {
                printf("DynaArray_get_position_by_element(al, &n1): success\n");
                printf("n=101,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8,n9=1,int o=1000\n");
                printf("Begin traverse index of DynaArray_edit_by_element(al, &n1):\n");
                LPDynaArray rows = (LPDynaArray)lde->data;
                StatusDataException_free(lde);
                for (long index = 0; index < rows->elements_num; index++)
                {
                    lde = DynaArray_get_by_position(rows, index);
                    printf("\tindex=%d, element_addr=%p, element_content=%d, element_content=%d\n", index, (long *)lde->data, *(long *)lde->data, *((long *)rows->elements + index));
                    StatusDataException_free(lde);
                }
                printf("End traverse DynaArray instance.\n");
            }
        }

        printf("n=101,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6,n7=7,n8=8\n");
        printf("Begin traverse DynaArray instance:\n");
        for (long index = 0; index < al->elements_num; index++)
        {
            lde = DynaArray_get_by_position(al, index);
            element = (int *)(lde->data);
            printf("\tindex=%d, element_addr=%p, element_content=%d, element_content=%d\n", index, element, *element, *((int *)al->elements + index));
            StatusDataException_free(lde);
        }
        printf("End traverse DynaArray instance.\n");

        DynaArray_free(al);
    }
}
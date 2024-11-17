#include <types.h>
#include <arraylist.h>
#include <stdio.h>

void test_arraylist();

int main()
{
    test_arraylist();
    return 0;
}

void test_arraylist()
{
    StatusDataError *lde = arraylist_new(10, sizeof(int));
    if (lde == NULL_POINTER)
        return;

    if (lde->status == NOTOK)
    {
        status_data_error_free(lde);
        printf("[E] arraylist_new\n");
        return;
    }

    ArrayList *al = (ArrayList *)(lde->data);
    if (al == NULL_POINTER)
    {
        status_data_error_free(lde);
        printf("[E] arraylist_new\n");
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

    status_data_error_free(lde);
    lde = NULL_POINTER;
    lde = arraylist_insert(al, 0, &n1);
    if (lde == NULL_POINTER)
    {
        arraylist_free(al);
        status_data_error_free(lde);

        printf("[E] arraylist_insert\n");
        return;
    }

    if (lde->status == NOTOK)
    {
        status_data_error_free(lde);
        arraylist_free(al);

        printf("[E] arraylist_insert\n");
        return;
    }

    status_data_error_free(lde);

    lde = arraylist_insert(al, 1, &n2);
    status_data_error_free(lde);

    lde = arraylist_insert(al, 2, &n3);
    status_data_error_free(lde);

    lde = arraylist_insert(al, 3, &n4);
    status_data_error_free(lde);

    lde = arraylist_insert(al, 4, &n5);
    status_data_error_free(lde);

    lde = arraylist_insert(al, 5, &n6);
    status_data_error_free(lde);

    lde = arraylist_insert(al, 6, &n7);
    status_data_error_free(lde);

    lde = arraylist_insert(al, 7, &n8);
    status_data_error_free(lde);

    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    lde = arraylist_iter(al);
    int *element;
    while (lde != NULL_POINTER && lde->error->error_iter_stop != YES)
    {
        element = (int *)(lde->data);
        printf("1 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        status_data_error_free(lde);
        lde = arraylist_iter(al);
    }
    // arraylist_iter_stop(al);

    status_data_error_free(lde);

    lde = arraylist_delete_element_by_position(al, 0);
    if (lde == NULL_POINTER)
    {
        arraylist_free(al);
        status_data_error_free(lde);
        lde = NULL_POINTER;

        printf("[E] arraylist_delete_element_by_position\n");
        return;
    }

    printf("After deleted al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    if (lde->status == NOTOK)
    {
        status_data_error_free(lde);
        arraylist_free(al);

        printf("[E] arraylist_delete_element_by_position\n");
        return;
    }
    status_data_error_free(lde);

    lde = arraylist_iter(al);
    while (lde != NULL_POINTER && lde->error->error_iter_stop == NO)
    {
        element = (int *)(lde->data);
        printf("2 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        status_data_error_free(lde);

        lde = arraylist_iter(al);
    }

    lde = arraylist_get_element_by_position(al, 8);
    if (lde == NULL_POINTER)
    {
        arraylist_free(al);

        printf("[E] arraylist_get_element_by_position\n");
        return;
    }
    element = (int *)lde->data;
    if (lde->error->error_index_out == NO)
        printf("al 1 element: %d\n", *element);
    else
        printf("al 8 element: index out\n");

    status_data_error_free(lde);

    printf("After edit by position\n");
    int n = 100;
    lde = arraylist_edit_element_by_position(al, 1, &n);
    status_data_error_free(lde);
    n = 101;

    lde = arraylist_iter(al);
    while (lde != NULL_POINTER && lde->error->error_iter_stop == NO)
    {
        element = (int *)(lde->data);
        printf("2 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        status_data_error_free(lde);
        lde = arraylist_iter(al);
    }

    lde = arraylist_edit_element_by_element(al, &n8, &n);
    printf("After edit by element, elements_num: %d\n", al->elements_num);

    status_data_error_free(lde);

    lde = arraylist_iter(al);
    while (lde != NULL_POINTER && lde->error->error_iter_stop == NO)
    {
        element = (int *)(lde->data);
        printf("3 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        status_data_error_free(lde);
        lde = arraylist_iter(al);
    }

    lde = arraylist_delete_element_by_element(al, &n);
    printf("After del by element, elements_num: %d\n", al->elements_num);

    status_data_error_free(lde);

    lde = arraylist_iter(al);
    while (lde != NULL_POINTER && lde->error->error_iter_stop == NO)
    {
        element = (int *)(lde->data);
        printf("4 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        status_data_error_free(lde);
        lde = arraylist_iter(al);
    }

    // 扩容测试
    int ns[10];
    for (int i = 0; i < 10; i++)
    {
        ns[i] = i;
        lde = arraylist_insert(al, al->elements_num, &ns[i]);
        status_data_error_free(lde);
    }

    printf("After add 10 elements, elements_num: %d, %p\n", al->elements_num, lde->error);

    lde = arraylist_iter(al);
    while (lde != NULL_POINTER && lde->error->error_iter_stop == NO)
    {
        element = (int *)(lde->data);
        printf("4 %d, %p, iter: %d\n", al->iter_index - 1, element, *element);
        status_data_error_free(lde);
        lde = arraylist_iter(al);
    }

    status_data_error_free(lde);
    arraylist_free(al);
    al = NULL_POINTER;
    lde = NULL_POINTER;
}
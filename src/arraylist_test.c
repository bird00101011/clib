#include <types.h>
#include <arraylist.h>
#include <stdio.h>

int main()
{
    StatusDataError *lde = arraylist_new(10, sizeof(int));
    if (lde == NULL_POINTER)
        return -1;

    if (lde->status == NOTOK)
    {
        printf("[E] arraylist_new, Error List: error_malloc:%d, error_memcpy:%d, error_memove:%d, \
               error_memset : % d, error_not_necessarily_operate : % d, error_realloc : % d\n ",
               lde->error->error_malloc,
               lde->error->error_memcpy,
               lde->error->error_memove, lde->error->error_memset,
               lde->error->error_index_out, lde->error->error_realloc);

        return -1;
    }

    ArrayList *al = (ArrayList *)(lde->data);
    if (al == NULL_POINTER)
        return -1;

    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    int n1 = 1;
    int n2 = 2;
    int n3 = 3;
    int n4 = 4;
    int n5 = 5;
    int n6 = 6;
    int n7 = 7;
    int n8 = 8;

    lde = arraylist_insert(al, 0, &n1);
    if (lde == NULL_POINTER)
        return -1;

    if (lde->status == NOTOK)
    {
        printf("[E] arraylist_insert, Error List: error_malloc:%d, error_memcpy:%d, error_memove:%d,\
                error_memset : % d, error_not_necessarily_operate : % d, error_realloc : % d\n ",
               lde->error->error_malloc,
               lde->error->error_memcpy,
               lde->error->error_memove, lde->error->error_memset,
               lde->error->error_index_out, lde->error->error_realloc);

        return -1;
    }

    arraylist_insert(al, 1, &n2);
    arraylist_insert(al, 2, &n3);
    arraylist_insert(al, 3, &n4);
    arraylist_insert(al, 4, &n5);
    arraylist_insert(al, 5, &n6);
    arraylist_insert(al, 6, &n7);
    arraylist_insert(al, 7, &n8);
    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    StatusDataError *isde = arraylist_iter(al);
    int *element;
    while (isde != NULL_POINTER && isde->error->error_iter_stop != YES)
    {
        element = (int *)(isde->data);
        printf("1 %d, %d, iter: %d\n", al->iter_index - 1, element, *element);
        isde = arraylist_iter(al);
    }
    arraylist_stop_iter(al);

    lde = arraylist_delete_element_by_position(al, 0);
    if (lde == NULL_POINTER)
    {
        return -1;
    }

    printf("1 al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    if (lde->status == NOTOK)
    {
        printf("[E] arraylist_insert, Error List: error_malloc:%d, error_memcpy:%d, error_memove:%d,\
                error_memset : % d, error_not_necessarily_operate : % d, error_realloc : % d\n ",
               lde->error->error_malloc,
               lde->error->error_memcpy,
               lde->error->error_memove, lde->error->error_memset,
               lde->error->error_index_out, lde->error->error_realloc);

        return -1;
    }

    isde = arraylist_iter(al);
    while (isde != NULL_POINTER && isde->error->error_iter_stop != YES)
    {
        element = (int *)(isde->data);
        printf("2 %d, %d, iter: %d\n", al->iter_index - 1, element, *element);
        isde = arraylist_iter(al);
    }

    free_status_data_error(lde);
    free_status_data_error(isde);
    return 0;
}
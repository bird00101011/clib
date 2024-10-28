#include <types.h>
#include <arraylist.h>
#include <stdio.h>

int main()
{
    StatusDataError *lde = arraylist_new(10, sizeof(int));
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
    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

    int n1 = 1;
    int n2 = 1;
    int n3 = 1;
    int n4 = 1;
    int n5 = 1;
    int n6 = 1;
    int n7 = 1;
    int n8 = 1;

    lde = arraylist_insert(al, 0, &n1);
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

    lde = arraylist_delete_element_by_position(al, 0);
    printf("al elements_num: %d, capacity: %d\n", al->elements_num, al->capacity);

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

    arraylist_free(al);
    free(lde->error);
    lde->data = NULL_POINTER;
    lde->error = NULL_POINTER;

    return 0;
}
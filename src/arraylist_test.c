#include <types.h>
#include <arraylist.h>
#include <stdio.h>

int main()
{
    StatusDataError *lde = arraylist_new(10, sizeof(int));
    if (lde->status == NOTOK)
    {
        printf("[E] arraylist_new, Error List: error_malloc:%d, error_memcpy:%d, error_memove:%d, error_memset:%d, error_not_necessarily_operate:%d, error_realloc:%d\n",
               lde->error->error_malloc, lde->error->error_memcpy,
               lde->error->error_memove, lde->error->error_memset,
               lde->error->error_not_necessarily_operate, lde->error->error_realloc);

        return -1;
    }

    ArrayList *al = (ArrayList *)(lde->data);
    printf("al elements_num: %d\n", al->elements_num);

    int n1 = 1;
    arraylist_insert(al, 0, &n1);
    printf("al elements_num: %d\n", al->elements_num);
    arraylist_free(al);
    
    free(lde->error);
    lde->data = NULL_POINTER;
    lde->error = NULL_POINTER;

    return 0;
}
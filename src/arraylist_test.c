#include <types.h>
#include <arraylist.h>
#include <stdio.h>

int main()
{
    StatusDataError *lde = arraylist_new(10, sizeof(int));
    arraylist_free(lde->data);
    free(lde->error);
    lde->error = NULL_POINTER;
    lde->error = NULL_POINTER;

    printf("ok.\n");

    return 0;
}
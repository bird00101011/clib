#include <arraylist.h>
#include <stdio.h>

int main()
{
    LPArrayList lpAl = arraylist_new(10, sizeof(int));
    arraylist_free(lpAl);
    printf("Ok\n");

    return 0;
}
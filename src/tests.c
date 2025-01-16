#include <error.h>
#include <dynaarray.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char *name;
    unsigned char age;
} Student, *LPStudent;

int copy_func(void *dst, void *src)
{
    LPStudent d = (LPStudent)dst;
    LPStudent s = (LPStudent)src;
    d->name = (char *)malloc(3);
    if (d->name != NULL_POINTER)
        strcpy(d->name, s->name);

    d->age = s->age;

    return TRUE;
}

int compare_func(void *dst, void *src)
{
    return TRUE;
}

int free_func(void *dst)
{
    LPStudent lp_s = (LPStudent)dst;
    free(lp_s->name);
    return TRUE;
}

void test_dynaarray()
{
    LPDynaArray lp_da = (LPDynaArray)malloc(sizeof(DynaArray));
    assert(lp_da != NULL_POINTER);

    long capacity, ele_size;
    capacity = 10;
    ele_size = sizeof(Student);
    int r;

    r = DynaArray_new(lp_da, capacity, ele_size, copy_func, compare_func, free_func);
    assert(r == TRUE);

    r = DynaArray_reallocate(lp_da, capacity * 2);
    assert(r == TRUE);

    char *mm = malloc(3);

    char *gg = malloc(3);

    char *jj = malloc(3);

    strcpy(mm, "MM");
    strcpy(gg, "GG");
    strcpy(jj, "JJ");

    Student smm = {mm, 16};
    Student sgg = {gg, 18};
    Student sjj = {jj, 17};

    printf("%d\n", lp_da->eles_num);

    r = DynaArray_insert(lp_da, lp_da->eles_num, &smm);
    assert(r == TRUE);

    r = DynaArray_insert(lp_da, lp_da->eles_num, &sgg);
    assert(r == TRUE);

    r = DynaArray_insert(lp_da, lp_da->eles_num, &sjj);
    assert(r == TRUE);

    DynaArray_del_by_pos(lp_da, 2);

    for (long i = 0; i < lp_da->eles_num; i++)
    {
        LPStudent s = (LPStudent)((char *)lp_da->eles + i * lp_da->ele_size);
        printf("%s, %d\n", s->name, s->age);
    }

    r = DynaArray_free(lp_da);
    assert(r == TRUE);
    free(lp_da);
}

int main()
{
    test_dynaarray();
    return 0;
}
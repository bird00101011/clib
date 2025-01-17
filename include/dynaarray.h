#ifndef DYNAARRAY_H
#define DYNAARRAY_H

typedef struct
{
    void *eles;
    long eles_num;
    long ele_size;
    long capacity;
    int (*copy_func)(void *, void *);
    int (*compare_func)(void *, void *);
    int (*free_func)(void *);
} DynaArray, *LPDynaArray;

int DynaArray_new(LPDynaArray lp_da,
                  long capacity,
                  long ele_size,
                  int (*copy_func)(void *, void *),
                  int (*compare_func)(void *, void *),
                  int (*free_func)(void *));
int DynaArray_free(LPDynaArray lp_da);

int DynaArray_reallocate(LPDynaArray lp_da, long new_capacity);

int DynaArray_insert(LPDynaArray lp_da, long pos, void *ele);

int DynaArray_del_by_pos(LPDynaArray lp_da, long pos);

int DynaArray_del_by_ele(LPDynaArray lp_da, void *ele);

int DynaArray_edit_by_pos(LPDynaArray lp_da, long pos, void *ele);

int DynaArray_edit_by_ele(LPDynaArray lp_da, void *old_ele, void *new_ele, LPDynaArray lp_poses);

int DynaArray_get_by_pos(LPDynaArray lp_da, long pos, void *ele);

int DynaArray_get_pos_by_ele(LPDynaArray lp_da, void *ele, LPDynaArray lp_poses);

#endif
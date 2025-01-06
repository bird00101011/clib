#include <types.h>
#ifndef ArrayList_H
#define ArrayList_H

typedef struct
{
    Object elements;
    long elements_num;
    long element_size;
    long capacity;
    long iter_index;
} ArrayList, *LPArrayList;

LPStatusDataException ArrayList_new(long capacity, long element_size);
LPStatusDataException ArrayList_free(LPArrayList lp_arraylist);

LPStatusDataException ArrayList_reallocate(LPArrayList lp_arraylist, long new_capacity);

LPStatusDataException ArrayList_insert(LPArrayList lp_arraylist, long position, Object element);

LPStatusDataException ArrayList_delete_element_by_position(LPArrayList lp_arraylist, long position);

LPStatusDataException ArrayList_iter(LPArrayList lp_arraylist);
LPStatusDataException ArrayList_iter_stop(LPArrayList lp_arraylist);

LPStatusDataException ArrayList_delete_element_by_element(LPArrayList lp_arraylist, Object element);

LPStatusDataException ArrayList_edit_element_by_position(LPArrayList lp_arraylist, long position, Object element);

LPStatusDataException ArrayList_edit_element_by_element(LPArrayList lp_arraylist, Object old_element, Object new_element);

LPStatusDataException ArrayList_get_element_by_position(LPArrayList lp_arraylist, long position);

LPStatusDataException ArrayList_get_position_by_element(LPArrayList lp_arraylist, Object element);

#endif
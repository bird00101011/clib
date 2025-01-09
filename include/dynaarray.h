#include <types.h>
#ifndef DYNAARRAY_H
#define DYNAARRAY_H

typedef struct
{
    Object elements;
    long elements_num;
    long element_size;
    long capacity;
} DynaArray, *LPDynaArray;

LPStatusDataException DynaArray_new(long capacity, long element_size);
LPStatusDataException DynaArray_free(LPDynaArray lp_dyna_array);

LPStatusDataException DynaArray_reallocate(LPDynaArray lp_dyna_array, long new_capacity);

LPStatusDataException DynaArray_insert(LPDynaArray lp_dyna_array, long position, Object element);

LPStatusDataException DynaArray_delete_by_position(LPDynaArray lp_dyna_array, long position);

LPStatusDataException DynaArray_delete_by_element(LPDynaArray lp_dyna_array, Object element);

LPStatusDataException DynaArray_edit_by_position(LPDynaArray lp_dyna_array, long position, Object element);

LPStatusDataException DynaArray_edit_by_element(LPDynaArray lp_dyna_array, Object old_element, Object new_element);

LPStatusDataException DynaArray_get_by_position(LPDynaArray lp_dyna_array, long position);

LPStatusDataException DynaArray_get_position_by_element(LPDynaArray lp_dyna_array, Object element);

#endif
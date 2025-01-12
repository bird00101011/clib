#include <types.h>
#include <dynaarray.h>
#include <linkedlist.h>
#include <hashmap.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>

LPStatusDataException HashMap_new()
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    LPHashMap lp_hm = malloc(sizeof(HashMap));
    if (lp_hm == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc = True;
        lp_sde->status = False;
        return lp_sde;
    }

    LPStatusDataException lp_sde_da = DynaArray_new(16, sizeof(HashMapNode));
    if (lp_sde_da == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_sde->status == False)
    {
        StatusDataException_free(lp_sde);
        return lp_sde_da;
    }

    lp_hm->items = (LPDynaArray)lp_sde_da->data;
    lp_sde->data = lp_hm;
    return lp_sde;
}

LPStatusDataException HashMap_free(LPHashMap lp_map)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_map == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    // TODO

    return lp_sde;
}

LPStatusDataException HashMap_gen_hash_code(LPHashMap lp_map, Object key, long key_size);
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_map == NULL_POINTER || key == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (key_size <= 0)
    {
        lp_sde->lp_exception->error_index_out = True;
        lp_sde->status = False;
        return lp_sde;
    }
    // TODO

    return lp_sde;
}

LPStatusDataException HashMap_put(LPHashMap lp_map, Object key, Object value)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_map == NULL_POINTER || key == NULL_POINTER || value == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    // TODO

    return lp_sde;
}

LPStatusDataException HashMap_get(LPHashMap lp_map, Object key)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_map == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    // TODO

    return lp_sde;
}

LPStatusDataException HashMap_delete(LPHashMap lp_map, Object key)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_map == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    // TODO

    return lp_sde;
}
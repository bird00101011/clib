#include <types.h>
#include <dynaarray.h>
#include <linkedlist.h>
#include <hashmap.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>

LPHashMapNode _hashmapnode_new(Object key, Object value, long key_size, long value_size)
{
    LPHashMapNode lp_hmn = malloc(sizeof(HashMapNode));
    if (lp_hmn == NULL_POINTER)
        return lp_hmn;

    if (key == NULL_POINTER)
    {
        lp_hmn->key = key;
        lp_hmn->key_size = 0;
    }
    else
    {
        lp_hmn->key = malloc(key_size);
        if (lp_hm->key == NULL_POINTER)
        {
            _hashmapnode_free(lp_hmn);
            lp_hmn = NULL_POINTER;
        }
    }

    if (value == NULL_POINTER)
    {
        lp_hmn->value = value;
        lp_hmn->value_size = 0;
    }
    else
    {
        lp_hmn->value = malloc(value_size);
        if (lp_hmn->value == NULL_POINTER)
        {
            _hashmapnode_free(lp_hmn);
            lp_hmn = NULL_POINTER;
        }
    }

    return lp_hmn;
}

void _hashmapnode_free(LPHashMapNode lp_hmn)
{
    if (lp_hmn != NULL_POINTER)
    {
        if (lp_hmn->key != NULL_POINTER)
            free(lp_hmn->key);

        if (lp_hmn->value != NULL_POINTER)
            free(lp_hmn->value);

        free(lp_hmn);
    }
}

Boolean _hashmap_init(LPHashMap lp_map)
{
    if (lp_map == NULL_POINTER)
        return False;

    LPHashMapNode lp_new = _hashmapnode_new(NULL_POINTER, NULL_POINTER, 0, 0);
    if (lp_new == NULL_POINTER)
    {
        return False;
    }

    LPStatusDataException lp_sde;
    Boolean errable = False;
    int i;

    for (i = 0; i < 16; i++)
    {
        lp_sde = DynaArray_insert(lp_map->items, lp_map->items->elements_num, lp_new);

        if (lp_sde == NULL_POINTER)
        {
            free(lp_new);
            errable = True;
            break;
        }

        if (lp_sde->status == False)
        {
            free(lp_new);
            StatusDataException_free(lp_sde);
            errable = True;
            break;
        }

        StatusDataException_free(lp_sde);
    }

    if (errable == False)
        return True;

    return False;
}

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
    StatusDataException_free(lp_sde_da);
    if (_hashmap_init(lp_hm) == False)
    {
        lp_sde->lp_exception->error_some++;
        lp_sde->status = False;
        HashMap_free(lp_hm);
        return lp_sde;
    }

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

    DynaArray_free();
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

    long ls = sizeof(long);
    lp_sde->data = malloc(ls);
    if (lp_sde->data == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc = True;
        lp_sde->status = False;
        return lp_sde;
    }

    long sum = 0;
    for (long i = 0; i < key_size; i++)
        sum += *(char *)(key + i);

    long hash_code = sum % lp_map->items->elements_capacity;
    if (memcpy((char *)lp_sde->data, (char *)&hash_code, ls) == NULL_POINTER)
    {
        lp_sde->lp_exception->error_memcpy = True;
        lp_sde->status = False;
    }

    return lp_sde;
}

LPStatusDataException HashMap_put(LPHashMap lp_map, Object key, long key_size, Object value, long value_size)
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

    LPStatusDataException lp_sde_hc = HashMap_gen_hash_code(lp_map, key, key_size);
    if (lp_sde_hc == NULL_POINTER)
    {
        StatusDataException_free(lp_sde_hc);
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_sde_hc->status == False)
    {
        StatusDataException_free(lp_sde);
        return lp_sde_hc;
    }

    long *lp_hash_code = (long *)lp_sde_hc->data;
    StatusDataException_free(lp_sde_hc);

    lp_sde_hc = DynaArray_get_by_position(lp_map->items, *lp_hash_code);
    if (lp_sde_hc == NULL_POINTER)
    {
        free(lp_hash_code);
        StatusDataException_free(lp_sde_hc);
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }

    if (lp_sde_hc->status == False)
    {
        free(lp_hash_code);
        StatusDataException_free(lp_sde);
        return lp_sde_hc;
    }

    StatusDataException_free(lp_sde_hc);
    LPHashMapNode lp_new = malloc(sizeof(HashMapNode));
    if (lp_new == NULL_POINTER)
    {
        free(lp_hash_code);
        lp_sde->lp_exception->error_malloc = True;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_new->key = memcpy((char *)lp_new->key, (char *)key, key_size);
    if (lp_new->key == NULL_POINTER)
    {
        free(lp_hash_code);
        free(lp_new);
        lp_sde->lp_exception->error_memcpy = True;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_new->value = memcpy((char *)lp_new->value, (char *)value, value_size);
    if (lp_new->value == NULL_POINTER)
    {
        free(lp_hash_code);
        free(lp_new->value);
        free(lp_new);
        lp_sde->lp_exception->error_memcpy = True;
        lp_sde->status = False;
        return lp_sde;
    }

    lp_new->key_size = key_size;
    lp_new->value_size = value_size;
    lp_sde_hc = DynaArray_edit_by_position(lp_map->items, *lp_hash_code, lp_new);
    if (lp_sde_hc == NULL_POINTER)
    {
        free(lp_hash_code);
        free(lp_new->key);
        free(lp_new->value);
        free(lp_new);
        StatusDataException_free(lp_sde_hc);
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        return lp_sde;
    }
    if (lp_sde_hc->status == False)
    {
        free(lp_hash_code);
        free(lp_new->key);
        free(lp_new->value);
        free(lp_new);
        StatusDataException_free(lp_sde);
        return lp_sde_hc;
    }

    lp_sde->data = lp_sde_hc->data;
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
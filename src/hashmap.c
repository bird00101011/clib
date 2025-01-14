#include <types.h>
#include <dynaarray.h>
#include <linkedlist.h>
#include <hashmap.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>

LPStatusDataException _hashmapnode_new(Object key, Object value, long key_size, long value_size)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    LPHashMapNode lp_hmn = malloc(sizeof(HashMapNode));
    if (lp_hmn == NULL_POINTER)
    {
        lp_sde->lp_exception->error_malloc = True;
        lp_sde->status = False;
        return lp_sde;
    }

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
            lp_sde->lp_exception->error_malloc = True;
            lp_sde->status = False;
        }
        else
        {
            if (memcpy((char *)lp_hmn->key, (char *)key, key_size) == NULL_POINTER)
            {
                _hashmapnode_free(lp_hmn);
                lp_sde->lp_exception->error_memcpy = True;
                lp_sde->status = False;
                return lp_sde;
            }
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
            lp_sde->lp_exception->error_malloc = True;
            lp_sde->status = False;
            return lp_sde;
        }
        else
        {
            if (memcpy((char *)lp_hmn->key, (char *)key, key_size) == NULL_POINTER)
            {
                _hashmapnode_free(lp_hmn);
                lp_sde->lp_exception->error_memcpy = True;
                lp_sde->status = False;
            }
        }
    }

    return lp_sde;
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

LPStatusDataException _hashmap_init(LPHashMap lp_map)
{
    LPStatusDataException lp_sde = StatusDataException_new();
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    if (lp_map == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        return lp_sde;
    }

    StatusDataException_free(lp_sde);
    lp_sde = LinkedList_new(sizeof(HashMapNode));
    if (lp_sde == NULL_POINTER || lp_sde->status == False)
        return lp_sde;

    LPLinkedList lp_ll = (LinkedList)lp_sde->data;
    StatusDataException_free(lp_sde);

    Boolean errable = False;
    for (int i = 0; i < 16; i++)
    {
        lp_sde = DynaArray_insert(lp_map->items, lp_map->items->elements_num, lp_ll);

        if (lp_sde == NULL_POINTER)
        {
            errable = True;
            break;
        }

        if (lp_sde->status == False)
        {
            StatusDataException_free(lp_sde);
            errable = True;
            break;
        }

        StatusDataException_free(lp_sde);
    }

    lp_sde = LinkedList_free(lp_ll);
    if (lp_sde == NULL_POINTER || lp_sde->status == False)
        return lp_sde;

    if (errable == False)
    {
        lp_sde->data = lp_map;
        lp_sde->status == True;
    }
    else
        lp_sde->status = False;

    return lp_sde;
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
    lp_sde_da = _hashmap_init(lp_hm);

    if (lp_sde_da == NULL_POINTER)
    {
        lp_sde->lp_exception->error_null_pointer = True;
        lp_sde->status = False;
        HashMap_free(lp_hm);
        return lp_sde;
    }

    if (lp_sde_da->status == False)
    {
        StatusDataException_free(lp_sde);
        HashMap_free(lp_hm);
        return lp_sde_da;
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

    LPStatusDataException lp_sde_iter;
    LPLinkedList lp_ll;
    LPLinkedListNode lp_hmn;
    for (long i = 0; i < lp_map->items->elements_num; i++)
    {
        lp_sde_iter = DynaArray_get_by_position(lp_map->items, i);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde->lp_exception->error_some++;
            continue;
        }
        if (lp_sde_iter->status == False)
        {
            StatusDataException_free(lp_sde_iter);
            lp_sde->lp_exception->error_some++;
            continue;
        }

        lp_ll = (LPLinkedList)lp_sde_iter->data;
        StatusDataException_free(lp_sde_iter);
        for (long j = 0; j < lp_ll->elements_num; j++)
        {
            lp_sde_iter = LinkedList_get_by_position(lp_ll, i);
            if (lp_sde_iter == NULL_POINTER)
            {
                lp_sde->lp_exception->error_some++;
                continue;
            }
            if (lp_sde_iter->status == False)
            {
                StatusDataException_free(lp_sde_iter);
                lp_sde->lp_exception->error_some++;
                continue;
            }
            lp_hmn = (LPLinkedList)lp_sde_iter->data;
            StatusDataException_free(lp_sde_iter);
            _hashmapnode_free(lp_hmn);
        }

        lp_sde_iter = LinkedList_free(lp_ll);
        if (lp_sde_iter == NULL_POINTER)
        {
            lp_sde->lp_exception->error_some++;
            continue;
        }

        if (lp_sde_iter->status == False)
            lp_sde->lp_exception->error_some++;

        StatusDataException_free(lp_sde_iter);
    }

    lp_sde_iter = DynaArray_free(lp_map->items);
    if (lp_sde_iter == NULL_POINTER)
        lp_sde->lp_exception->error_some++;

    if (lp_sde_iter->status == False)
    {
        StatusDataException_free(lp_sde_iter);
        lp_sde->lp_exception->error_some++;
    }

    if (lp_sde->lp_exception->error_some != 0)
    {
        lp_sde->status = False;
        return lp_sde;
    }

    lp_sde->data = lp_map;
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
        sum += *(unsigned char *)(key + i);

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
    // TODO

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
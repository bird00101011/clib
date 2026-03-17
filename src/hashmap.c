#include <assert.h>
#include <dynaarray.h>
#include <error.h>
#include <hashmap.h>
#include <linkedlist.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

int da_copy_func(void* dst, void* src) {
  LPLinkedList d = (LPLinkedList)dst;
  LPLinkedList s = (LPLinkedList)src;

  return TRUE;
}

int da_compare_func(void* dst, void* src) { return TRUE; }
int da_free_func(void* dst) { return TRUE; }

__declspec(dllexport) int HashMap_init(LPHashMap lp_hm,
                                       int (*copy_func)(void*, void*),
                                       int (*compare_func)(void*, void*),
                                       int (*free_func)(void*)) {
  assert(lp_hm != NULL_POINTER);

  lp_hm->lp_da_lls = malloc(sizeof(LPDynaArray));

  if (lp_hm->lp_da_lls == NULL_POINTER) {
    set_last_error(CLIB_MALLOC_FAILED);
    return FALSE;
  }

  char ll_size = sizeof(LinkedList);
  if (DynaArray_init(lp_hm->lp_da_lls, TABLE_SIZE, ll_size, da_copy_func,
                     da_compare_func, da_free_func) == FALSE) {
    return FALSE;
  }

  lp_hm->copy_func = copy_func;
  lp_hm->compare_func = compare_func;
  lp_hm->free_func = free_func;

  LinkedList ll;
  char hmn_size = sizeof(HashMapNode);
  if (LinkedList_init(&ll, hmn_size, copy_func, compare_func, free_func) ==
      FALSE) {
    DynaArray_free(lp_hm->lp_da_lls);
    return FALSE;
  }

  char err = FALSE;
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (DynaArray_insert(lp_hm->lp_da_lls, lp_hm->lp_da_lls->eles_num, &ll) ==
        FALSE) {
      err = TRUE;
      break;
    }
  }
  if (err == TRUE) {
    DynaArray_free(lp_hm->lp_da_lls);
    return FALSE;
  }

  return TRUE;
}

__declspec(dllexport) unsigned long HashMap_gen_hashcode(void* key,
                                                         long key_size) {
  char* k = (char*)key;
  unsigned long hash = 0;
  for (long i = 0; i < key_size; i++) hash += (hash << 5) + k[i];

  return hash;
}

__declspec(dllexport) int HashMap_put(LPHashMap lp_hm, LPHashMapNode lp_hmn) {
  assert(lp_hm != NULL_POINTER && lp_hmn != NULL_POINTER &&
         lp_hmn->key != NULL_POINTER && lp_hmn->value != NULL_POINTER);

  if (lp_hmn->key_size <= 0 || lp_hmn->value_size <= 0) {
    set_last_error(CLIB_PARAMS_WRONG);
    return FALSE;
  }

  unsigned long hash = HashMap_gen_hashcode(lp_hmn->key, lp_hmn->key_size);
  // 2 % 4 =2, 2 & (4 - 1) = 2
  unsigned long index = hash & (lp_hm->lp_da_lls->capacity - 1);

  LPLinkedList bucket =
      (LPLinkedList)DynaArray_get_addr_by_pos(lp_hm->lp_da_lls, index);

  if (bucket == NULL_POINTER) {
    set_last_error(CLIB_NULL_POINTER_FAILED);
    return FALSE;
  }

  if (bucket->eles_num == 0) {
    LinkedList_insert(bucket, bucket->eles_num, lp_hmn->value);
  }

  return TRUE;
}

__declspec(dllexport) int HashMap_get(LPHashMap lp_hm, LPHashMapNode lpmn);

__declspec(dllexport) int HashMap_del(LPHashMap lp_hm, void* key,
                                      long key_size);

__declspec(dllexport) int HashMap_free(LPHashMap lp_hm);
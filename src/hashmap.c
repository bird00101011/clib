#include <assert.h>
#include <dynaarray.h>
#include <error.h>
#include <hashmap.h>
#include <linkedlist.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

int copy_func(void *, void *);
int compare_func(void *, void *);
int free_func(void *);

__declspec(dllexport) int HashMap_init(LPHashMap lp_hm,
                                       int (*copy_func)(void *, void *),
                                       int (*compare_func)(void *, void *),
                                       int (*free_func)(void *)) {
  assert(lp_hm != NULL_POINTER);

  lp_hm->lp_da_lls = malloc(sizeof(LPDynaArray));

  if (lp_hm->lp_da_lls == NULL_POINTER) {
    set_last_error(CLIB_MALLOC_FAILED);
    return FALSE;
  }

  char capacity = 16;
  char ll_size = sizeof(LinkedList);
  if (DynaArray_init(lp_hm->lp_da_lls, capacity, ll_size, copy_func,
                     compare_func, free_func) == FALSE) {
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
  for (int i = 0; i < 16; i++) {
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
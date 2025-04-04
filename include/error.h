#ifndef ERROR_H
#define ERROR_H

#define CLIB_MALLOC_FAILED 10000
#define CLIB_REALLOC_FAILED 10001
#define CLIB_MEMSET_FAILED 10002
#define CLIB_MEMOVE_FAILED 10003
#define CLIB_MEMCPY_FAILED 10004
#define CLIB_INDEX_OUT_FAILED 10005
#define CLIB_NULL_POINTER_FAILED 10006
#define CLIB_CALLBACKFUNC_FAILED 10007
#define CLIB_PARAMS_WRONG 10008
#define CLIB_HASHMAP_REALLOC_FAILED 10009

#define NULL_POINTER (void *)0
#define TRUE 0
#define FALSE -1

long get_last_error();
void set_last_error(long n);

#endif
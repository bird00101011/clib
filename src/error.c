#include <error.h>

#define NULL_POINTER (void *)0

long CLIB_LAST_ERROR = 0;

long get_last_error()
{
    return CLIB_LAST_ERROR;
}

void set_last_error(long n)
{
    CLIB_LAST_ERROR = n;
}
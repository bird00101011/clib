#include <error.h>

long CLIB_LAST_ERROR = 0;

__declspec(dllexport)
long get_last_error()
{
    return CLIB_LAST_ERROR;
}

void set_last_error(long n)
{
    CLIB_LAST_ERROR = n;
}
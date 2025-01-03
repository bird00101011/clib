#include <types.h>
#include <stdlib.h>

void cliberror_init(CLIBError *ce)
{
    if (ce != NULL_POINTER)
    {
        ce->error_index_out = NO;
        ce->error_iter_stop = NO;
        ce->error_malloc = NO;
        ce->error_memcpy = NO;
        ce->error_memove = NO;
        ce->error_memset = NO;
        ce->error_null_pointer = NO;
        ce->error_realloc = NO;
        ce->error_memcmp = NO;
    }
}

StatusDataError *status_data_error_new()
{
    StatusDataError *lde = malloc(sizeof(StatusDataError));
    if (lde == NULL_POINTER)
    {
        return lde;
    }

    lde->error = malloc(sizeof(CLIBError));
    cliberror_init(lde->error);
    lde->data = NULL_POINTER;

    if (lde->error == NULL_POINTER)
    {
        free(lde);
        lde = NULL_POINTER;
        return lde;
    }

    return lde;
}

void status_data_error_free(StatusDataError *sde)
{
    if (sde != NULL_POINTER)
    {
        if (sde->error != NULL_POINTER)
            free(sde->error);

        // 由于data不知是不是用malloc创建的，所以需要用户自己决定如何释放
        sde->data = NULL_POINTER;
        sde->error = NULL_POINTER;

        free(sde);
    }
}
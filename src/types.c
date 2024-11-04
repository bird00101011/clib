#include <types.h>

void init_error(CLIBError *ce)
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
    }
}

StatusDataError *init_status_data_error()
{
    StatusDataError *lde = malloc(sizeof(StatusDataError));
    if (lde == NULL_POINTER)
    {
        return lde;
    }

    lde->error = malloc(sizeof(CLIBError));
    init_error(lde->error);

    if (lde->error == NULL_POINTER)
    {
        free(lde);
        lde = NULL_POINTER;
        return lde;
    }

    return lde;
}

void free_status_data_error(StatusDataError *sde)
{
    if (sde != NULL_POINTER)
    {
        if (sde->error != NULL_POINTER)
            free(sde->error);
        if (sde->data != NULL_POINTER)
            free(sde->data);

        sde->data = NULL_POINTER;
        sde->error = NULL_POINTER;

        free(sde);
        sde = NULL_POINTER;
    }
}
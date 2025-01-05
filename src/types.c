#include <types.h>
#include <stdlib.h>

LPStatusDataException exception_new()
{
    LPExcepiton lp_e = malloc(sizeof(Exception));
    if (lp_e != NULL_POINTER)
    {
        lp_e->error_index_out = False;
        lp_e->error_iter_stop = False;
        lp_e->error_malloc = False;
        lp_e->error_memcpy = False;
        lp_e->error_memove = False;
        lp_e->error_memset = False;
        lp_e->error_null_pointer = False;
        lp_e->error_realloc = False;
        lp_e->error_memcmp = False;
    }
    LPStatusDataException lp_sde = 
    return lp_e;
}

StatusDataError *status_data_error_new()
{
    StatusDataError *lde = malloc(sizeof(StatusDataError));
    if (lde == NULL_POINTER)
    {
        return lde;
    }

    lde->error = malloc(sizeof(CLIBError));
    exception_init(lde->error);
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
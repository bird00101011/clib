#include <types.h>
#include <stdlib.h>

LPException Exception_new()
{
    LPException lp_e = malloc(sizeof(Exception));
    if (lp_e != NULL_POINTER)
    {
        lp_e->error_index_out = False;
        lp_e->error_some = 0;
        lp_e->error_malloc = False;
        lp_e->error_memcpy = False;
        lp_e->error_memove = False;
        lp_e->error_memset = False;
        lp_e->error_null_pointer = False;
        lp_e->error_realloc = False;
        lp_e->error_memcmp = False;
    }

    return lp_e;
}

void Exception_free(LPException e)
{
    if (e != NULL_POINTER)
        free(e);
}

LPStatusDataException StatusDataException_new()
{
    LPStatusDataException lp_sde = malloc(sizeof(StatusDataException));
    if (lp_sde == NULL_POINTER)
        return lp_sde;

    lp_sde->lp_exception = Exception_new();
    if (lp_sde->lp_exception == NULL_POINTER)
    {
        free(lp_sde);
        return NULL_POINTER;
    }

    lp_sde->data = NULL_POINTER;
    lp_sde->status = True;
    return lp_sde;
}

void StatusDataException_free(LPStatusDataException lp_sde)
{
    if (lp_sde != NULL_POINTER)
    {
        if (lp_sde->lp_exception != NULL_POINTER)
            Exception_free(lp_sde->lp_exception);

        // 由于data不知是不是用malloc创建的，所以需要用户自己决定如何释放
        free(lp_sde);
    }
}
#include <types.h>
#include <stdlib.h>

LPException Exception_new()
{
    LPException lp_e = malloc(sizeof(Exception));
    if (lp_e == NULL_POINTER)
        return lp_e;

    lp_e->error_index_out = 0;
    lp_e->error_callback = 0;
    lp_e->error_malloc = 0;
    lp_e->error_memcpy = 0;
    lp_e->error_memove = 0;
    lp_e->error_memset = 0;
    lp_e->error_null_pointer = 0;
    lp_e->error_realloc = 0;

    return lp_e;
}

void Exception_free(LPException e)
{
    if (e == NULL_POINTER)
        return;

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
    if (lp_sde == NULL_POINTER)
        return;

    if (lp_sde->lp_exception != NULL_POINTER)
        Exception_free(lp_sde->lp_exception);

    // Since the data is not created with malloc, it is up to the user to decide how to release it
    free(lp_sde);
}
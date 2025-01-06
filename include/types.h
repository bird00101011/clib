#ifndef TYPES_H
#define TYPES_H

typedef(void *) Object;
#define NULL_POINTER (Object 0)

typedef enum
{
    True = 1,
    False = 0
} Boolean;

typedef struct
{
    Boolean error_malloc;
    Boolean error_realloc;
    Boolean error_memset;
    Boolean error_memove;
    Boolean error_memcpy;
    Boolean error_memcmp;
    Boolean error_index_out;
    Boolean error_null_pointer;
    Boolean error_iter_stop;
} Exception, *LPExcepiton;

typedef struct
{
    Boolean status;
    Object data;
    LPException lp_exception;
} StatusDataException, *LPStatusDataException;

LPStatusDataException StatusDataException_new();
void StatusDataException_free(LPStatusDataException lp_sde);
LPException Exception_new();
void Exception_free(LPException e);

#endif
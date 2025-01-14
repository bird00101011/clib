// The project base type
#ifndef TYPES_H
#define TYPES_H

typedef void *Object;
#define NULL_POINTER (Object)0

typedef enum
{
    True = 1,
    False = 0
} Boolean;

typedef struct
{
    long error_malloc;
    long error_realloc;
    long error_memset;
    long error_memove;
    long error_memcpy;
    long error_memcmp;
    long error_index_out;
    long error_null_pointer;
    long error_callback;
} Exception, *LPException;

LPException Exception_new();
void Exception_free(LPException e);

typedef struct
{
    Boolean status;
    Object data;
    LPException lp_exception;
} StatusDataException, *LPStatusDataException;

LPStatusDataException StatusDataException_new();
void StatusDataException_free(LPStatusDataException lp_sde);

#endif
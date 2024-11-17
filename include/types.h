#ifndef TYPES_H
#define TYPES_H

#define NULL_POINTER ((void *)0)

typedef unsigned char Byte;

typedef enum
{
    OK = 1,
    NOTOK = 0
} OkOrNotOk;

typedef enum
{
    YES = 1,
    NO = 0
} YesOrNo;

typedef struct
{
    Byte error_malloc;
    Byte error_realloc;
    Byte error_memset;
    Byte error_memove;
    Byte error_memcpy;
    Byte error_memcmp;
    Byte error_index_out;
    Byte error_null_pointer;
    Byte error_iter_stop;
} CLIBError;

typedef struct
{
    OkOrNotOk status;
    void *data;
    CLIBError *error;
} StatusDataError;

StatusDataError *status_data_error_new();
void status_data_error_free(StatusDataError *sde);
void cliberror_init(CLIBError *ce);

#endif
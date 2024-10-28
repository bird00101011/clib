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
    Byte error_index_out;
    Byte error_null_pointer;
} CLIBError;

typedef struct
{
    OkOrNotOk status;
    void *data;
    CLIBError *error;
} StatusDataError;

#if defined(WINDOWS)
#include <windows.h>
#endif

typedef struct
{
    void *elements;    // 保存数据的数组
    long elements_num; // 列表存储的元素个数
    long element_size; // 单个元素字节数
    long capacity;     // 列表容量
#if defined(WINDOWS)
    HANDLE g_mutex; // Windows下的互斥锁
#endif
} ArrayList;

#endif
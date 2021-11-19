#ifndef EVIL_VEC_H
#define EVIL_VEC_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t vec_size(void* vec);
void* vec_create();
void vec_free(void* vec);

#define vec_reserve(vec_name, val)                                       \
    {                                                                    \
        size_t _n_bytes = sizeof(typeof(*vec_name)) * val;               \
        vec_wrapper_resize(vec_name, sizeof(size_t) * 2 + 1 + _n_bytes); \
    };

#define vec_push(vec, val)                                                                  \
    {                                                                                       \
        typeof(*vec) _val = val;                                                            \
        size_t _val_size = sizeof(typeof(*vec));                                            \
        if (vec_size(vec) * _val_size + 2 * sizeof(size_t) + 1 >= vec_allocated_bytes(vec)) \
            vec = vec_wrapper_resize(vec, vec_allocated_bytes(vec) * 2 + _val_size);        \
        vec[vec_size(vec)] = _val;                                                          \
        ((size_t*)vec_get_wrapper(vec))[0]++;                                               \
    };

#endif

//------------------------------------------------------------------------------------------------------------------------------------
// Vec Utils
typedef char byte;

byte* vec_get_wrapper(void* vec)
{
    return ((byte*)vec - 2 * sizeof(size_t));
}

size_t vec_size(void* vec)
{
    return ((size_t*)vec_get_wrapper(vec))[0];
}

size_t vec_allocated_bytes(void* vec)
{
    return ((size_t*)vec_get_wrapper(vec))[1];
}
//------------------------------------------------------------------------------------------------------------------------------------
// Con- Destructor
void* vec_create()
{
    byte* vec_wrapper = (byte*)malloc(sizeof(size_t) * 2 + 1);
    ((size_t*)vec_wrapper)[0] = 0;                      // size
    ((size_t*)vec_wrapper)[1] = sizeof(size_t) * 2 + 1; // allocated bytes;
    return &vec_wrapper[sizeof(size_t) * 2];
}

void vec_free(void* vec)
{
    free(vec_get_wrapper(vec));
    vec = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------
// General Usage
void* vec_wrapper_resize(void* vec, size_t n_bytes)
{
    byte* vec_wrapper = vec_get_wrapper(vec);
    ((size_t*)vec_wrapper)[1] = n_bytes;
    vec_wrapper = (byte*)realloc(vec_wrapper, vec_allocated_bytes(vec));
    return &vec_wrapper[sizeof(size_t) * 2];
}
// TODO: vec_rem
// TODO: vec_insert

//------------------------------------------------------------------------------------------------------------------------------------

#ifndef EVIL_VEC_H
#define EVIL_VEC_H
#include <stdlib.h>
#include <string.h>
// TODO: unconditional jumps ??
//
#define _VEC_WRAPPER_SIZE sizeof(size_t) * 2 + 1

static size_t vec_size(void* vec);
static void* vec_create();
static void vec_free(void* vec);
typedef char byte;

static void _vec_rem(byte* vec, size_t index, size_t type_size);
#define vec_rem(vec, index) _vec_rem((byte*)vec, index, sizeof(*vec));

#define vec_reserve(vec_name, val)                                  \
    {                                                               \
        size_t _n_bytes = sizeof(*vec_name) * val;                  \
        vec_wrapper_resize(vec_name, _VEC_WRAPPER_SIZE + _n_bytes); \
    };

// TODO: get rid of macro magic
#define vec_push(vec, val)                                                             \
    {                                                                                  \
        size_t _val_size = sizeof(*vec);                                               \
        if (vec_size(vec) * _val_size + _VEC_WRAPPER_SIZE >= vec_allocated_bytes(vec)) \
            vec = vec_wrapper_resize(vec, vec_allocated_bytes(vec) * 2 + _val_size);   \
        vec[vec_size(vec)] = val;                                                      \
        ((size_t*)vec_get_wrapper(vec))[0]++;                                          \
    };

#endif

#ifndef EVIL_VEC_H_IMPLEMENTATION
#define EVIL_VEC_H_IMPLEMENTATION

//------------------------------------------------------------------------------------------------------------------------------------
// Vec Utils
typedef char byte;

static byte* vec_get_wrapper(void* vec)
{
    return ((byte*)vec - 2 * sizeof(size_t));
}

static size_t vec_size(void* vec)
{
    return ((size_t*)vec_get_wrapper(vec))[0];
}

static size_t vec_allocated_bytes(void* vec)
{
    return ((size_t*)vec_get_wrapper(vec))[1];
}
//------------------------------------------------------------------------------------------------------------------------------------
// Con- Destructor
static void* vec_create()
{
    byte* vec_wrapper = (byte*)malloc(_VEC_WRAPPER_SIZE);
    ((size_t*)vec_wrapper)[0] = 0;                 // size
    ((size_t*)vec_wrapper)[1] = _VEC_WRAPPER_SIZE; // allocated bytes;
    return &vec_wrapper[sizeof(size_t) * 2];
}

static void vec_free(void* vec)
{
    free(vec_get_wrapper(vec));
    vec = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------
// General Usage
static void* vec_wrapper_resize(void* vec, size_t n_bytes)
{
    byte* vec_wrapper = vec_get_wrapper(vec);
    ((size_t*)vec_wrapper)[1] = n_bytes;
    vec_wrapper = (byte*)realloc(vec_wrapper, vec_allocated_bytes(vec));
    return &vec_wrapper[sizeof(size_t) * 2];
}

static void _vec_rem(byte* vec, size_t index, size_t type_size)
{
    memcpy(&vec[type_size * index], &vec[(index + 1) * type_size], vec_size((void*)vec) * type_size);
    ((size_t*)vec_get_wrapper(vec))[0]--;
}
// TODO: vec_insert
// -->  for (int i = 1; i < vec_size(vec) + 1; i++) {
//          temp = vec[i];
//          vec[i] = store;
//          store = temp;
//      }
//      something like this
//------------------------------------------------------------------------------------------------------------------------------------
#endif

#ifndef CLIB_H
#define CLIB_H

// TODO: include cstdlib when using cpp
#include "stdlib.h"

#ifdef CLIB_DEBUG
#define CLIB_LOG( ... ) printf( __VA_ARGS__ )
#else
#define CLIB_LOG( ... )
#endif

#ifdef __cplusplus
extern "C"{
#endif

typedef void (*callback)();

typedef struct clib_param
{
    const char flag_letter;
    const char flag_word[20];
    const char description[100];
    callback callback_fct;
} clib_param;

typedef struct clib_result
{
    clib_param    **params;
    size_t          count;
} clib_parsing_result;

extern void                       clib_init();
extern void                       clib_shutdown();
extern clib_param*                clib_find_param_with_letter(char letter);
extern void                       clib_say_hello();
extern const clib_parsing_result* clib_parse(int argc, const char **argv);
extern void                       clib_decl_param(const clib_param* );
extern void                       clib_decl_params(int param_count, const clib_param* param_vector[] );
extern void                       clib_resize_register(size_t desired_size);

#ifdef __cplusplus
}
#endif

#endif //CLIB_H

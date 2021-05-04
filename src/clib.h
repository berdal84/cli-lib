#ifndef CLIB_H
#define CLIB_H

#ifdef __cplusplus
#   include <cstdlib>
#else
#   include "stdlib.h"
#endif

#ifdef CLIB_DEBUG
#include <stdio.h>
#define CLIB_LOG( ... ) printf( __VA_ARGS__ )
#else
#define CLIB_LOG( ... )
#endif

#ifdef __cplusplus
extern "C"{
#endif

/** struct to store a single parameter */
typedef struct clib_param
{
    char flag_letter;
    char flag_word[20];
    char description[100];
    void (*callback_fct)();
} clib_param;

/** struct to store an array of parameters */
typedef struct clib_params
{
    clib_param*  data;
    size_t       size;
    size_t       capacity;
} clib_params;

extern void                       clib_init();
extern void                       clib_shutdown();
extern const clib_param*          clib_find_param_with_word(const char* word);
extern const clib_param*          clib_find_param_with_letter(char letter);
extern void                       clib_say_hello();
extern const clib_params*         clib_parse(int argc, const char **argv);
extern void                       clib_decl_param(const clib_param* );
extern void                       clib_decl_params(int param_count, const clib_param* param_vector[] );
extern void clib_grow_buffer(clib_params *buffer, size_t amount);

#ifdef __cplusplus
}
#endif

#endif //CLIB_H

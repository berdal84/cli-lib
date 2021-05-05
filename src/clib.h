#ifndef CLIB_H
#define CLIB_H

#ifdef __cplusplus
#   include <cstdlib>
#else
#   include "stdlib.h"
#endif

#ifdef CLIB_DEBUG
#include <stdio.h>
#define CLIB_LOG( ... ) printf( "[clib] "__VA_ARGS__ )
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
} Param;

/** struct to store an array of parameters */
typedef struct clib_params
{
    Param*  data;
    size_t  size;
    size_t  capacity;
} Params;

// Public API --------------------------------------------------------------------------
extern void            clib_decl_param(const Param* );
extern void            clib_decl_params(int param_count, const Param* param_vector[] );
extern const Param*    clib_find_param_with_letter(char letter);
extern const Param*    clib_find_param_with_word(const char* word);
extern void            clib_init();
extern const Params*   clib_parse(int argc, const char **argv);
extern void            clib_print_status();
extern void            clib_shutdown();
extern int             clib_param_cmp(const Param* lhs, const Param* rhs);

// Internal ----------------------------------------------------------------------------
extern const Params*   clib_get_params();
extern void            clib_buffer_alloc(Params* buffer, size_t nb_elem_to_reserve);
extern void            clib_buffer_free(Params* buffer);
extern void            clib_buffer_grow(Params *buffer, size_t amount);

#ifdef __cplusplus
}
#endif

#endif //CLIB_H

#ifndef CLI_LIB_H
#define CLI_LIB_H

#include "stdlib.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef void (*callback)();

typedef struct clilib_param
{
    const char flag_letter;
    const char flag_word[20];
    const char description[100];
    callback callback_fct;
} clilib_param;

typedef struct clilib_result
{
    clilib_param    **params;
    size_t          count;
} clilib_parsing_result;

extern void            clilib_init();
extern void            clilib_shutdown();
extern clilib_param*   clilib_find_param_with_letter(char letter);
extern void            clilib_say_hello();
extern const clilib_parsing_result* clilib_parse(int argc, const char **argv);
extern void            clilib_decl_param(const clilib_param* );
extern void            clilib_decl_params(int param_count, const clilib_param* param_vector[] );
extern void            clilib_resize_register(size_t desired_size);

#ifdef __cplusplus
}
#endif

#endif //CLI_LIB_H

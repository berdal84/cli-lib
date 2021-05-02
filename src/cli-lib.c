#include "cli-lib.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

static clilib_param** registry      = NULL;
static size_t         registry_size = 0;

void clilib_say_hello()
{
    printf("Hello, World!\n");
}

clilib_parsing_result clilib_parse(int argc, const char **argv)
{
    printf("clilib parsing (%i arguments)...\n", argc);

    clilib_parsing_result result = {
        malloc(sizeof( clilib_param* ) ),
        0
    };

    printf("clilib info: ignoring first param (binary path)\n");
    int arg_idx;
    for (arg_idx = 1; arg_idx < argc; ++arg_idx )
    {
        printf("clilib parsing argument %i : ", arg_idx);

        const char *arg = argv[arg_idx];
        size_t length = strlen(arg);
        if( length >= 2 && arg[0] == '-' )
        {
            // is a flag (ex: "-f" or "-h")
            if ( arg[1] != '-' && length == 2 )
            {
                clilib_param* found = clilib_find_param_with_letter(arg[1]);
                if ( found != NULL )
                {
                    ++(result.count);
                    // TODO: realloc better to reduce allocation count (ex: 1, 2, 4, 8, etc.)
                    result.params = realloc(result.params, result.count * sizeof( clilib_param* ) );
                    result.params[result.count - 1] = found;
                    printf("OK.\n");
                }
                else
                {
                    printf("ignored. Reason: no parameter found for this flag.\n");
                }
            }
            // TODO: long flag (ex: "--force" or "--help")
            else
            {
                printf("ignored. Reason: double dash flag are not yet implemented.\n");
            }
        }
        else
        {
            printf("ignored. Reason: argument is not starting with a dash (\"-\" or \"--\"), it is not a flag.\n");
        }
    }

    printf("clilib detected %lu param(s)\n", result.count);
    printf("clilib will call callback_fct on each (if defined)...\n");

    for(arg_idx = 0; arg_idx < result.count ; ++arg_idx)
    {
        if(result.params[arg_idx]->callback_fct != NULL )
        {
            printf("clilib is calling --%s flag's call_back_fct.\n", result.params[arg_idx]->flag_word);

            (result.params[arg_idx]->callback_fct)();
        }
    }

    printf("clilib parsing done (result.count = %lu).\n", result.count);

    return result;
}

void clilib_decl_param(const clilib_param* param)
{
    clilib_resize_register(registry_size + 1);

    void* each = malloc(sizeof(clilib_param));
    memcpy( each,  param, sizeof(clilib_param ) );
    registry[registry_size-1] = each;
}

void clilib_resize_register(size_t desired_size)
{
    if ( desired_size == registry_size)
        return;

    size_t bytes_required = desired_size * sizeof( clilib_param* );

    if ( registry_size != 0 )
    {
        registry = realloc(registry, bytes_required);
    }
    else
    {
        registry = malloc(bytes_required);
    }

    registry_size = desired_size;
}

void clilib_decl_params( int param_count, const clilib_param* param_vector[])
{
    clilib_resize_register(registry_size + param_count);

    int i = 0;
    while(i < param_count )
    {
        void* each = malloc( sizeof( clilib_param) );
        memcpy( each,  param_vector[i++], sizeof( clilib_param ) );
        registry[registry_size + i] = each;
    }
    registry_size += param_count;
}

clilib_param* clilib_find_param_with_letter(const char letter)
{
    int i = 0;
    while(i < registry_size )
    {
        if (registry[i]->flag_letter == letter )
            return registry[i];
        ++i;
    }
    return NULL;
}

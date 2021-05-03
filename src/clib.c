#include "clib.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

static clib_param**        registry;
static size_t                registry_size;
static clib_parsing_result last_parsing_result;

void clib_init()
{
    CLIB_LOG("clib initialization...\n");

    registry      = NULL;
    registry_size = 0;
    last_parsing_result.params = NULL;
    last_parsing_result.count  = 0;

    CLIB_LOG("clib is initialized.\n");
}

void clib_shutdown()
{
    CLIB_LOG("clib is shutting down...\n");

    size_t i;
    for( i = 0; i < registry_size; ++i)
    {
        CLIB_LOG(" - freeing registry[%lu]\n", i);
        free(registry[i]);
    }
    CLIB_LOG(" - freeing registry\n");

    free(registry);

    registry_size = 0;

    CLIB_LOG("clib is shutdown.\n");
}

void clib_say_hello()
{
    CLIB_LOG("clib says: Hello, World!\n");
}

const clib_parsing_result* clib_parse(int argc, const char **argv)
{
    CLIB_LOG("clib parsing (%i arguments)...\n", argc);

    free( last_parsing_result.params );

    last_parsing_result.params = malloc(0);
    last_parsing_result.count = 0;

    CLIB_LOG("clib info: ignoring first param (binary path)\n");
    int arg_idx;
    for (arg_idx = 1; arg_idx < argc; ++arg_idx )
    {
        CLIB_LOG("clib parsing argument %i : ", arg_idx);

        const char *arg = argv[arg_idx];
        size_t length = strlen(arg);
        if( length >= 2 && arg[0] == '-' )
        {
            // is a flag (ex: "-f" or "-h")
            if ( arg[1] != '-' && length == 2 )
            {
                clib_param* found = clib_find_param_with_letter(arg[1]);
                if ( found != NULL )
                {
                    ++(last_parsing_result.count);
                    // TODO: realloc better to reduce allocation count (ex: 1, 2, 4, 8, etc.)
                    last_parsing_result.params = realloc(last_parsing_result.params, last_parsing_result.count * sizeof( clib_param* ) );
                    last_parsing_result.params[last_parsing_result.count - 1] = found;
                    CLIB_LOG("OK.\n");
                }
                else
                {
                    CLIB_LOG("ignored. Reason: no parameter found for this flag.\n");
                }
            }
            // TODO: long flag (ex: "--force" or "--help")
            else
            {
                CLIB_LOG("ignored. Reason: double dash flag are not yet implemented.\n");
            }
        }
        else
        {
            CLIB_LOG("ignored. Reason: argument is not starting with a dash (\"-\" or \"--\"), it is not a flag.\n");
        }
    }

    CLIB_LOG("clib detected %lu param(s)\n", last_parsing_result.count);
    CLIB_LOG("clib will call callback_fct on each (if defined)...\n");

    for(arg_idx = 0; arg_idx < last_parsing_result.count ; ++arg_idx)
    {
        if(last_parsing_result.params[arg_idx]->callback_fct != NULL )
        {
            CLIB_LOG("clib is calling --%s flag's call_back_fct.\n", last_parsing_result.params[arg_idx]->flag_word);

            (last_parsing_result.params[arg_idx]->callback_fct)();
        }
    }

    CLIB_LOG("clib parsing done (last_parsing_result.count = %lu).\n", last_parsing_result.count);

    return &last_parsing_result;
}

void clib_decl_param(const clib_param* param)
{
    clib_resize_register(registry_size + 1);

    void* each = malloc(sizeof(clib_param));
    memcpy( each,  param, sizeof(clib_param ) );
    registry[registry_size-1] = each;
}

void clib_resize_register(size_t desired_size)
{
    if ( desired_size == registry_size)
        return;

    size_t bytes_required = desired_size * sizeof( clib_param* );

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

void clib_decl_params( int param_count, const clib_param* param_vector[])
{
    clib_resize_register(registry_size + param_count);

    int i = 0;
    while(i < param_count )
    {
        void* each = malloc( sizeof( clib_param) );
        memcpy( each,  param_vector[i++], sizeof( clib_param ) );
        registry[registry_size + i] = each;
    }
    registry_size += param_count;
}

clib_param* clib_find_param_with_letter(const char letter)
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

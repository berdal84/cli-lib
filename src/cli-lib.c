#include "cli-lib.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

static clilib_param**        registry;
static size_t                registry_size;
static clilib_parsing_result last_parsing_result;

void clilib_init()
{
    CLILIB_LOG("clilib initialization...\n");

    registry      = NULL;
    registry_size = 0;
    last_parsing_result.params = NULL;
    last_parsing_result.count  = 0;

    CLILIB_LOG("clilib is initialized.\n");
}

void clilib_shutdown()
{
    CLILIB_LOG("clilib is shutting down...\n");

    size_t i;
    for( i = 0; i < registry_size; ++i)
    {
        CLILIB_LOG(" - freeing registry[%lu]\n", i);
        free(registry[i]);
    }
    CLILIB_LOG(" - freeing registry\n");

    free(registry);

    registry_size = 0;

    CLILIB_LOG("clilib is shutdown.\n");
}

void clilib_say_hello()
{
    CLILIB_LOG("clilib says: Hello, World!\n");
}

const clilib_parsing_result* clilib_parse(int argc, const char **argv)
{
    CLILIB_LOG("clilib parsing (%i arguments)...\n", argc);

    free( last_parsing_result.params );

    last_parsing_result.params = malloc(0);
    last_parsing_result.count = 0;

    CLILIB_LOG("clilib info: ignoring first param (binary path)\n");
    int arg_idx;
    for (arg_idx = 1; arg_idx < argc; ++arg_idx )
    {
        CLILIB_LOG("clilib parsing argument %i : ", arg_idx);

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
                    ++(last_parsing_result.count);
                    // TODO: realloc better to reduce allocation count (ex: 1, 2, 4, 8, etc.)
                    last_parsing_result.params = realloc(last_parsing_result.params, last_parsing_result.count * sizeof( clilib_param* ) );
                    last_parsing_result.params[last_parsing_result.count - 1] = found;
                    CLILIB_LOG("OK.\n");
                }
                else
                {
                    CLILIB_LOG("ignored. Reason: no parameter found for this flag.\n");
                }
            }
            // TODO: long flag (ex: "--force" or "--help")
            else
            {
                CLILIB_LOG("ignored. Reason: double dash flag are not yet implemented.\n");
            }
        }
        else
        {
            CLILIB_LOG("ignored. Reason: argument is not starting with a dash (\"-\" or \"--\"), it is not a flag.\n");
        }
    }

    CLILIB_LOG("clilib detected %lu param(s)\n", last_parsing_result.count);
    CLILIB_LOG("clilib will call callback_fct on each (if defined)...\n");

    for(arg_idx = 0; arg_idx < last_parsing_result.count ; ++arg_idx)
    {
        if(last_parsing_result.params[arg_idx]->callback_fct != NULL )
        {
            CLILIB_LOG("clilib is calling --%s flag's call_back_fct.\n", last_parsing_result.params[arg_idx]->flag_word);

            (last_parsing_result.params[arg_idx]->callback_fct)();
        }
    }

    CLILIB_LOG("clilib parsing done (last_parsing_result.count = %lu).\n", last_parsing_result.count);

    return &last_parsing_result;
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

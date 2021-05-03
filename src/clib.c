#include "clib.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

static clib_params registry;
static clib_params last_parsing_result;

void clib_init()
{
    CLIB_LOG("clib initialization...\n");

    registry.data = malloc( sizeof( clib_param* ) );
    registry.size = 0;
    last_parsing_result.data = malloc( sizeof( clib_param* ) );
    last_parsing_result.size  = 0;

    CLIB_LOG("clib is initialized.\n");
}

void clib_shutdown()
{
    CLIB_LOG("clib is shutting down...\n");

    // free registry
    size_t i;
    for( i = 0; i < registry.size; ++i)
    {
        CLIB_LOG(" - freeing registry[%lu]\n", i);
        free((void*)registry.data[i]);
    }
    CLIB_LOG(" - freeing registry\n");
    free(registry.data);
    registry.size = 0;

    // free last result
    free(last_parsing_result.data);
    last_parsing_result.size = 0;

    CLIB_LOG("clib is shutdown.\n");
}

void clib_say_hello()
{
    CLIB_LOG("clib says: Hello, World!\n");
}

const clib_params* clib_parse(int argc, const char **argv)
{
    CLIB_LOG("clib parsing (%i arguments)...\n", argc);
    last_parsing_result.size = 0;

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
                const clib_param* found = clib_find_param_with_letter(arg[1]);
                if ( found != NULL )
                {
                    ++(last_parsing_result.size);
                    // TODO: realloc better to reduce allocation count (ex: 1, 2, 4, 8, etc.)
                    last_parsing_result.data = reallocarray(last_parsing_result.data, last_parsing_result.size, sizeof( clib_param* ) );
                    last_parsing_result.data[last_parsing_result.size - 1] = found;
                    CLIB_LOG("OK. (flag is -%s )\n", found->flag_word);
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

    CLIB_LOG("clib detected %lu param(s)\n", last_parsing_result.size);
    CLIB_LOG("clib will call callback_fct on each (if defined)...\n");

    for(arg_idx = 0; arg_idx < last_parsing_result.size ; ++arg_idx)
    {
        if(last_parsing_result.data[arg_idx]->callback_fct != NULL )
        {
            CLIB_LOG("clib is calling --%s flag's call_back_fct.\n", last_parsing_result.data[arg_idx]->flag_word);

            (last_parsing_result.data[arg_idx]->callback_fct)();
        }
    }

    CLIB_LOG("clib parsing done (last_parsing_result.count = %lu).\n", last_parsing_result.size);

    return &last_parsing_result;
}

void clib_decl_param(const clib_param* param)
{
    clib_resize_register(registry.size + 1);

    clib_param* each = malloc( sizeof(clib_param) );
    memcpy( each,  param, sizeof(clib_param ) );
    registry.data[registry.size - 1] = each;
}

void clib_resize_register(size_t desired_size)
{
    if ( desired_size == registry.size)
        return;

    registry.data = reallocarray(registry.data, desired_size, sizeof( clib_param* ));
    registry.size = desired_size;
}

void clib_decl_params( int param_count, const clib_param* param_vector[])
{
    clib_resize_register(registry.size + param_count);

    int i = 0;
    while(i < param_count )
    {
        void* each = malloc( sizeof( clib_param) );
        memcpy( each,  param_vector[i++], sizeof( clib_param ) );
        registry.data[registry.size + i] = each;
    }
    registry.size += param_count;
}

const clib_param* clib_find_param_with_letter(const char letter)
{
    int i = 0;
    while( i < registry.size )
    {
        if (registry.data[i]->flag_letter == letter )
            return registry.data[i];
        ++i;
    }
    return NULL;
}

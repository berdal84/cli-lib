#include "clib.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static clib_params registry;
static clib_params parse_res;

void clib_create_buffer(clib_params* buffer, size_t reserve)
{
    buffer->capacity = reserve;
    buffer->data = calloc(reserve, sizeof( clib_param ) );
    buffer->size = 0;

    if( buffer->data == NULL )
    {
        CLIB_LOG("Unable to initialize pointers !\n");
        exit(1);
    }
}

void clib_init()
{
    CLIB_LOG("clib initialization...\n");

    clib_create_buffer(&registry, 1);
    clib_create_buffer(&parse_res, 1);

    CLIB_LOG("clib is initialized.\n");
}

void clib_shutdown()
{
    CLIB_LOG("clib is shutting down...\n");

    // free registry
    free(registry.data);
    registry.size = 0;
    registry.capacity = 0;

    // free last result
    free(parse_res.data);
    parse_res.size = 0;
    parse_res.capacity = 0;

    CLIB_LOG("clib is shutdown.\n");
}

void clib_say_hello()
{
    CLIB_LOG("clib says: Hello, World!\n");
}

const clib_params* clib_parse(int argc, const char **argv)
{
    CLIB_LOG("clib parsing (%i arguments)...\n", argc);
    parse_res.size = 0;

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
                    clib_grow_buffer(&parse_res, 1);
                    parse_res.data[parse_res.size - 1] = *found;
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

    CLIB_LOG("clib detected %lu param(s)\n", parse_res.size);
    CLIB_LOG("clib will call callback_fct on each (if defined)...\n");

    for(arg_idx = 0; arg_idx < parse_res.size ; ++arg_idx)
    {
        if(parse_res.data[arg_idx].callback_fct != NULL )
        {
            CLIB_LOG("clib is calling --%s flag's call_back_fct.\n", parse_res.data[arg_idx].flag_word);

            (parse_res.data[arg_idx].callback_fct)();
        }
    }

    CLIB_LOG("clib parsing done (parse_res.count = %lu).\n", parse_res.size);

    return &parse_res;
}

void clib_decl_param(const clib_param* param)
{
    clib_grow_buffer(&registry, 1);
    registry.data[registry.size - 1] = *param;
}

void clib_grow_buffer(clib_params *buffer, size_t amount)
{
    assert( amount != 0); // "Why growing with 0 amount ?"

    if ( buffer->capacity < amount + buffer->size  ) // need to allocate more ?
    {
        buffer->capacity = buffer->capacity == 0 ? 1 : buffer->capacity * 2;
        buffer->data = reallocarray( buffer->data, buffer->capacity, sizeof( clib_param ));

        if ( buffer->data == NULL)
        {
            CLIB_LOG("Unable to resize_buffer !\n");
            exit(1);
        }
    }

    registry.size += amount;
}

void clib_decl_params( int param_count, const clib_param* param_vector[])
{
    clib_grow_buffer(0, registry.size + param_count);

    int i = 0;
    while(i < param_count )
    {
        memcpy( &registry.data[registry.size + i],  param_vector[i++], sizeof( clib_param ) );
    }
    registry.size += param_count;
}

const clib_param* clib_find_param_with_letter(const char letter)
{
    int i = 0;
    while( i < registry.size )
    {
        if (registry.data[i].flag_letter == letter )
            return &registry.data[i];
        ++i;
    }
    return NULL;
}

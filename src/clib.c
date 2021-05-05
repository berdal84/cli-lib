#include "clib.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef enum {
    Status_SHUTDOWN = 0,
    Status_READY,
    Status_PARSING,
    Status_COUNT
} Status;

static Status clib_status = Status_SHUTDOWN;
static const char* clib_status_strings[Status_COUNT] = {
    [Status_SHUTDOWN] = "Status_SHUTDOWN",
    [Status_READY]    = "Status_READY",
    [Status_PARSING]  = "Status_PARSING"
};
static Params registry;
static Params parse_res;

inline size_t min(size_t a, size_t b) { return a < b ? a : b; }

void clib_buffer_alloc(Params* buffer, size_t nb_elem_to_reserve)
{
    buffer->capacity = nb_elem_to_reserve;
    buffer->data = malloc(nb_elem_to_reserve * sizeof( Param ) );
    buffer->size = 0;

    if( buffer->data == NULL )
    {
        CLIB_LOG("Unable to initialize pointers !\n");
        exit(1);
    }
}

void clib_buffer_free(Params* buffer)
{
    free(buffer->data);
    buffer->size = 0;
    buffer->capacity = 0;
}

void clib_buffer_append(Params* buffer, const Param* elem_to_append)
{
    clib_buffer_grow_size(buffer, 1);
    buffer->data[parse_res.size - 1] = *elem_to_append;
}

void clib_init()
{
    assert(clib_status == Status_SHUTDOWN);
    CLIB_LOG("initialization...\n");
    clib_buffer_alloc(&registry, 1);
    clib_buffer_alloc(&parse_res, 1);
    clib_status = Status_READY;
    clib_print_status();
}

void clib_shutdown()
{
    assert(clib_status == Status_READY);
    CLIB_LOG("is shutting down...\n");
    clib_buffer_free(&registry);
    clib_buffer_free(&parse_res);
    clib_status = Status_SHUTDOWN;
    clib_print_status();
}

void clib_print_status()
{
    CLIB_LOG("is %s.\n", clib_status_strings[clib_status]);
}

const Params* clib_parse(int argc, const char **argv)
{
    clib_status = Status_PARSING;

    CLIB_LOG("parsing (%i arguments)...\n", argc);
    CLIB_LOG("info: ignoring first param (binary path)\n");
    int arg_idx;
    for ( arg_idx = 1; arg_idx < argc; ++arg_idx )
    {
        const char *arg = argv[arg_idx];
        CLIB_LOG("is parsing argument  \"%s\" (idx: %i.) ", arg, arg_idx);

        size_t length = strlen(arg);

        // is short flag ? (ex: "-f", "-fg" or "-fgh")
        if( length >= 2 && arg[0] == '-' && arg[1] != '-')
        {
            size_t arg_char_index = 1;
            const Param* found = clib_find_param_with_letter(arg[arg_char_index]);

            if ( found != NULL )
            {
                CLIB_LOG("Found: ");
                while (found != NULL) {
                    clib_buffer_append(&parse_res, found);
                    CLIB_LOG("-%c ", found->flag_letter);

                    found = clib_find_param_with_letter(arg[++arg_char_index]);
                }
                CLIB_LOG("\n");
            }
            else
            {
                CLIB_LOG("Nothing found\n");
            }
        }
        // is flag long ? (ex: --my-flag, --mute or --help)
        else if ( length > 2 && strncmp(arg, "--", 2) == 0 )
        {
            const Param* found = clib_find_param_with_word(&arg[2]);
            if ( found != NULL )
            {
                clib_buffer_append(&parse_res, found);
                CLIB_LOG("Found: --%s\n", found->flag_word);
            }
            else
            {
                CLIB_LOG("Nothing found\n");
            }
        }
        else
        {
            CLIB_LOG("ignored. Reason: argument is not starting with a dash (\"-\" or \"--\"), it is not a flag.\n");
        }
    }

    CLIB_LOG("detected %lu param(s)\n", parse_res.size);
    CLIB_LOG("will call callback_fct on each (if defined)...\n");

    for ( arg_idx = 0; arg_idx < parse_res.size ; ++arg_idx )
    {
        if ( parse_res.data[arg_idx].callback_fct != NULL )
        {
            CLIB_LOG("is calling --%s flag's call_back_fct.\n", parse_res.data[arg_idx].flag_word);

            (parse_res.data[arg_idx].callback_fct)();
        }
    }

    CLIB_LOG("parsing done (parse_res.count = %lu).\n", parse_res.size);

    clib_status = Status_READY;

    return &parse_res;
}

void clib_decl_param(const Param* param)
{
    clib_buffer_grow_size(&registry, 1);
    registry.data[registry.size - 1] = *param;
}

const Params* clib_get_params()
{
    if( clib_status == Status_SHUTDOWN )
        CLIB_LOG("Please, init clib with clib_init() before doing anything else.");
    return &registry;
}

void clib_buffer_grow_size(Params *buffer, size_t amount)
{
    assert( amount != 0); // "Why growing with 0 amount ?"
    buffer->size += amount;
    if ( buffer->capacity < buffer->size  ) // need to allocate more ?
    {
        /*
         * Capacity growing pattern:
         * - call 1: we allocate for the exact amount asked.
         * - call n+1: double capacity with a grow limited to CLIB_BUFFER_CAPACITY_GROW_MAX
         */
        buffer->capacity = buffer->capacity == 0 ? buffer->size :  buffer->capacity + min( buffer->capacity, CLIB_BUFFER_CAPACITY_GROW_MAX ) ;
        buffer->data     = reallocarray( buffer->data, buffer->capacity, sizeof( Param ));

        if ( buffer->data == NULL)
        {
            CLIB_LOG("Unable to resize_buffer !\n");
            exit(1);
        }
    }
}

void clib_decl_params( int param_count, const Param* param_vector[])
{
    clib_buffer_grow_size(0, registry.size + param_count);

    int i = 0;
    while(i < param_count )
    {
        memcpy( &registry.data[registry.size + i],  param_vector[i], sizeof( Param ) );
        i++;
    }
    registry.size += param_count;
}

const Param* clib_find_param_with_word(const char* word)
{
    int i = 0;
    while( i < registry.size )
    {
        if ( strcmp(registry.data[i].flag_word, word) == 0)
            return &registry.data[i];
        ++i;
    }
    return NULL;
}

const Param* clib_find_param_with_letter(const char letter)
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

int clib_param_cmp(const Param *lhs, const Param *rhs)
{
    return lhs->flag_letter != rhs->flag_letter
        || strcmp(lhs->flag_word, rhs->flag_word) != 0
        || lhs->callback_fct != rhs->callback_fct
        || strcmp(lhs->description, rhs->description) != 0;
}

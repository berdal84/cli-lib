#include <iostream>
#include "clib.h"

void help_message()
{
    std::cout << "Hello, this message is displayed because you set the flag -h or --help." << std::endl;
}

int main(int argc, const char** argv )
{
    // mem init
    clib_init();

    // setup clilib with some params
    struct clib_param param = {
        'h',
        "help",
        "show program help",
        help_message
    };
    clib_decl_param(&param);

    // "parse"
    clib_parse(argc, argv);

    // mem free
    clib_shutdown();

    return 0;
}

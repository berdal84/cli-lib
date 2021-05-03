#include <iostream>
#include "cli-lib.h"

void help_message()
{
    std::cout << "Hello, this message is displayed because you set the flag -h or --help." << std::endl;
}

int main(int argc, const char** argv )
{
    // mem init
    clilib_init();

    // setup clilib with some params
    struct clilib_param param = {
        'h',
        "help",
        "show program help",
        help_message
    };
    clilib_decl_param(&param);

    // "parse"
    clilib_param** result = nullptr;
    clilib_parse(argc, argv);

    // mem free
    clilib_shutdown();

    return 0;
}

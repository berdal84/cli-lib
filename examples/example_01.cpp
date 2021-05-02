#include <iostream>
#include "cli-lib.h"

int main(int argc, const char** argv )
{
    // setup clilib with some params
    clilib::params params{{
        'h',
        "help",
        "show program help",
        []() {
            std::cout << "Hello, this message is displayed because you set the flag -h or --help." << std::endl;
        }
    }};
    clilib::decl_params(params);

    // "parse"
    clilib::params result;
    clilib::parse(argc, argv, result);

    return 0;
}

#include <iostream>
#include "cli-lib.h"

int main(int argc, const char** argv ) {

    // setup clilib with some parameters
    clilib::param show_help_param = {
        'h',
        "help",
        "show program help",
        []() {
            std::cout << "Hello, this message is displayed because you set the flag -h or --help." << std::endl;
        }
    };
    clilib::declare_param(show_help_param);

    std::vector<const clilib::param*> result;
    clilib::parse(argc, argv, result);

    return 0;
}

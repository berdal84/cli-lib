#include "cli-lib.h"
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace clilib::internal;

void clilib::say_hello()
{
    std::cout << "Hello, World!" << std::endl;
}

bool clilib::parse(const char **argv, int argc, std::vector<const clilib::param*> &outResult)
{
    for (int i = 1; i < argc; ++i )
    {
        const char *arg = argv[i];
        size_t length = strlen(arg);
        if( length >= 2 && arg[0] == '-' )
        {
            // is a flag ?  (ex: "-f" or "-r"
            if ( arg[1] != '-' && length == 2 )
            {
                auto found = std::find_if( params_registry.begin(), params_registry.end(), [&arg](param& item) { return  item.flag_letter == arg[1];  } );
                if ( found != params_registry.end() )
                    outResult.push_back(&(*found));
            }
            else
            {
                std::cout << "unable to parse param " << i << ". Reason: is not a flag." << std::endl;
            }
        }
    }
    return true;
}

void clilib::declare_param(clilib::param p)
{
    params_registry.emplace_back(p);
}

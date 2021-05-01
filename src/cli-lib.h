#ifndef CLI_LIB_H
#define CLI_LIB_H

#include <vector>
#include <string>

namespace clilib
{
    struct param
    {
        const char flag_letter;
        const char flag_word[20];
        const char description[50];
    };

    void say_hello();
    bool parse(const char **argv, int argc, std::vector<const clilib::param*> &outResult);
    void declare_param(clilib::param);

    namespace internal {
        static std::vector<clilib::param> params_registry;
    }
}

#endif //CLI_LIB_H

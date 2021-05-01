#ifndef CLI_LIB_H
#define CLI_LIB_H

#include <vector>
#include <string>
#include <functional>

namespace clilib
{
    struct param
    {
        const char flag_letter;
        const char flag_word[20];
        const char description[50];
        std::function<void()> action;
    };

    void say_hello();
    bool parse(int argc, const char **argv, std::vector<const clilib::param *> &outResult);
    void declare_param(clilib::param);

    namespace internal {
        static std::vector<clilib::param> params_registry;
    }
}

#endif //CLI_LIB_H

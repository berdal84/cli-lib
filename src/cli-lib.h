#ifndef CLI_LIB_H
#define CLI_LIB_H

#include <utility>
#include <vector>
#include <string>
#include <functional>

namespace clilib
{

    struct param
    {
        const char flag_letter;
        std::string flag_word;
        std::string description;
        std::function<void()> action;
    };

    typedef std::vector<clilib::param> params;

    void say_hello();
    bool parse(int argc, const char **argv, params& outResult);
    void decl_param(const param& );
    void decl_params(params&);
    namespace internal {
        static params params_registry;
    }
}

#endif //CLI_LIB_H

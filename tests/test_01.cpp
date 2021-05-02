#include "gtest/gtest.h"
#include "cli-lib.h"

namespace
{
    void f(){ std::cout << "fullscreen on." << std::endl; };
    void g(){ std::cout << "gradient on." << std::endl; };

    TEST(Parse, SimpleFlag)
    {

        // prepare clilib with some expected result
        clilib_param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clilib_param g_param = { 'g', "gradient", "enable gradient ui", g };
        clilib_decl_param(&f_param);
        clilib_decl_param(&g_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "-f", "f", "-g"};
        int argc = sizeof(args) / sizeof(const char*);

        // act
        clilib_parsing_result result = clilib_parse(argc, args);

        // test
        EXPECT_TRUE(result.params);
        EXPECT_STREQ(result.params[0]->flag_word, f_param.flag_word);
        EXPECT_STREQ(result.params[1]->flag_word, g_param.flag_word);
        EXPECT_EQ(result.count, 2);
    }

}  // namespace
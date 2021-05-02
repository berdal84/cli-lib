#include "gtest/gtest.h"
#include "cli-lib.h"

namespace
{
    TEST(Parse, SimpleFlag)
    {
        // prepare clilib with some expected params
        clilib::param fullscreen_param = { 'f', "fullscreen", "toggle fullscreen automatically" };
        clilib::param g_param = { 'g', "gradient", "enable gradient ui" };
        clilib::decl_param(fullscreen_param);
        clilib::decl_param(g_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        clilib::params params;
        const char* args[]{"executable", "-f", "f", "-g"};
        int argc = sizeof(args) / sizeof(const char*);

        // act
        clilib::parse(argc, args, params);

        // test
        EXPECT_EQ(params.size(), 2);
        EXPECT_EQ(params.at(0).flag_word, fullscreen_param.flag_word);
        EXPECT_EQ(params.at(1).flag_word, g_param.flag_word);
    }

}  // namespace
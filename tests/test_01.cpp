#include "gtest/gtest.h"
#include "cli-lib.h"

namespace
{
    TEST(Parse, SimpleFlag)
    {
        // prepare clilib with some expected params
        clilib::param fullscreen_param = { 'f', "fullscreen", "toggle fullscreen automatically" };
        clilib::param g_param = { 'g', "gradient", "enable gradient ui" };
        clilib::declare_param(fullscreen_param);
        clilib::declare_param(g_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        std::vector<const clilib::param*> params;
        const char* args[4];
        args[0] = "executable";
        args[1] = "-f";
        args[2] = "f";  // this must be ignored.
        args[3] = "-g"; // this must be ignored since no param correspond to this.
        int argc = 4;

        // act
        clilib::parse(argc, args, params);

        // test
        EXPECT_EQ(params.size(), 2);
        EXPECT_STREQ(params.at(0)->flag_word, fullscreen_param.flag_word);
        EXPECT_STREQ(params.at(1)->flag_word, g_param.flag_word);
    }

}  // namespace
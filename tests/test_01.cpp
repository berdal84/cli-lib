#include "gtest/gtest.h"
#include "clib.h"

namespace
{
    void f(){ std::cout << "fullscreen on." << std::endl; };
    void g(){ std::cout << "gradient on." << std::endl; };

    TEST(Parse, SingleShortFlag)
    {
        clib_init();

        clib_param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clib_decl_param(&f_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "-f"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const clib_params* result = clib_parse(argc, args);

        // test
        EXPECT_TRUE(result->data);
        EXPECT_STREQ(result->data[0].flag_word, f_param.flag_word);
        EXPECT_EQ(result->size, 1);

        // free mem
        clib_shutdown();
    }

    TEST(Parse, TwoShortFlags)
    {
        clib_init();

        // prepare clilib with some expected result
        clib_param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clib_param g_param = { 'g', "gradient", "enable gradient ui", g };
        clib_decl_param(&f_param);
        clib_decl_param(&g_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "-g", "-f"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const clib_params* result = clib_parse(argc, args);

        // test
        EXPECT_TRUE(result->data);
        EXPECT_STREQ(result->data[0].flag_word, g_param.flag_word);
        EXPECT_STREQ(result->data[1].flag_word, f_param.flag_word);
        EXPECT_EQ(result->size, 2);

        // free mem
        clib_shutdown();
    }

    TEST(Parse, TwoShortFlagsInRow)
    {
        clib_init();

        // prepare clilib with some expected result
        clib_param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clib_param g_param = { 'g', "gradient", "enable gradient ui", g };
        clib_decl_param(&f_param);
        clib_decl_param(&g_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "-gf"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const clib_params* result = clib_parse(argc, args);

        // test
        EXPECT_TRUE(result->data);
        EXPECT_STREQ(result->data[0].flag_word, g_param.flag_word);
        EXPECT_STREQ(result->data[1].flag_word, f_param.flag_word);
        EXPECT_EQ(result->size, 2);

        // free mem
        clib_shutdown();
    }

    TEST(Parse, SingleLongFlag)
    {
        clib_init();

        clib_param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clib_decl_param(&f_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "--fullscreen"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const clib_params* result = clib_parse(argc, args);

        // test
        EXPECT_TRUE(result->data);
        EXPECT_STREQ(result->data[0].flag_word, f_param.flag_word);
        EXPECT_EQ(result->size, 1);

        // free mem
        clib_shutdown();
    }

}  // namespace
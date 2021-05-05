#include "gtest/gtest.h"
#include "clib.h"

namespace
{
    void f(){ std::cout << "fullscreen on." << std::endl; };
    void g(){ std::cout << "gradient on." << std::endl; };

    TEST(API, param_cmp)
    {
        Param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        Param f_param_cpy = f_param;

        // test
        EXPECT_FALSE(clib_param_cmp(&f_param, &f_param_cpy));
    }

    TEST(API, buffer_grow)
    {
        clib_init();
        Params params;
        clib_buffer_alloc(&params, 0);
        EXPECT_EQ(params.capacity, 0);

        clib_buffer_grow_size(&params, 1);
        EXPECT_EQ(params.capacity, 1);

        clib_buffer_grow_size(&params, 1);
        EXPECT_EQ(params.capacity, 2);

        clib_buffer_grow_size(&params, 1);
        EXPECT_EQ(params.capacity, 4);

        clib_shutdown();
    }

    TEST(API, buffer_grow_fast)
    {
        clib_init();
        Params buffer;
        clib_buffer_alloc(&buffer, CLIB_BUFFER_CAPACITY_GROW_MAX / 2 );
        EXPECT_EQ(buffer.size, 0);
        EXPECT_EQ(buffer.capacity, CLIB_BUFFER_CAPACITY_GROW_MAX / 2);

        clib_buffer_grow_size(&buffer, CLIB_BUFFER_CAPACITY_GROW_MAX / 2);
        EXPECT_EQ(buffer.size, CLIB_BUFFER_CAPACITY_GROW_MAX / 2);
        EXPECT_EQ(buffer.capacity, buffer.size);

        clib_buffer_grow_size(&buffer, CLIB_BUFFER_CAPACITY_GROW_MAX / 2 );
        EXPECT_EQ(buffer.size, CLIB_BUFFER_CAPACITY_GROW_MAX );
        EXPECT_EQ(buffer.capacity, buffer.size); // should double

        clib_buffer_grow_size(&buffer, CLIB_BUFFER_CAPACITY_GROW_MAX / 2);
        EXPECT_EQ(buffer.size, CLIB_BUFFER_CAPACITY_GROW_MAX + CLIB_BUFFER_CAPACITY_GROW_MAX / 2);
        EXPECT_EQ(buffer.capacity, CLIB_BUFFER_CAPACITY_GROW_MAX * 2 ); // should double again

        clib_buffer_grow_size(&buffer, CLIB_BUFFER_CAPACITY_GROW_MAX / 2);
        EXPECT_EQ(buffer.size, CLIB_BUFFER_CAPACITY_GROW_MAX * 2 );
        EXPECT_EQ(buffer.capacity, buffer.size );

        clib_buffer_grow_size(&buffer, CLIB_BUFFER_CAPACITY_GROW_MAX / 2);
        EXPECT_EQ(buffer.size, CLIB_BUFFER_CAPACITY_GROW_MAX * 2 + CLIB_BUFFER_CAPACITY_GROW_MAX / 2);
        EXPECT_LT(buffer.size, buffer.capacity);

        clib_shutdown();
    }

    TEST(API, buffer_grow_super_fast)
    {
        clib_init();
        Params buffer;

        clib_buffer_alloc(&buffer, 0 );
        EXPECT_EQ(buffer.size, 0);
        EXPECT_EQ(buffer.capacity, 0);

        clib_buffer_grow_size(&buffer, CLIB_BUFFER_CAPACITY_GROW_MAX * 1024 );
        EXPECT_EQ(buffer.size, CLIB_BUFFER_CAPACITY_GROW_MAX * 1024);
        EXPECT_EQ(buffer.capacity, CLIB_BUFFER_CAPACITY_GROW_MAX * 1024);

        clib_buffer_grow_size(&buffer, 1);
        EXPECT_EQ(buffer.size, CLIB_BUFFER_CAPACITY_GROW_MAX * 1024 + 1);
        EXPECT_EQ(buffer.capacity, CLIB_BUFFER_CAPACITY_GROW_MAX * 1024 + CLIB_BUFFER_CAPACITY_GROW_MAX);

        clib_shutdown();
    }

    TEST(Declare, ASingleParam)
    {
        // prepare
        clib_init();

        // act
        Param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clib_decl_param(&f_param);

        // test
        const Params* params = clib_get_params();
        EXPECT_EQ(params->size, 1);
        EXPECT_TRUE(params->data);
        EXPECT_FALSE(clib_param_cmp(&params->data[0], &f_param));

        // free mem
        clib_shutdown();
    }


    TEST(Parse, SingleShortFlag)
    {
        clib_init();

        Param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clib_decl_param(&f_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "-f"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const Params* result = clib_parse(argc, args);

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
        Param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        Param g_param = {'g', "gradient", "enable gradient ui", g };
        clib_decl_param(&f_param);
        clib_decl_param(&g_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "-g", "-f"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const Params* result = clib_parse(argc, args);

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
        Param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        Param g_param = {'g', "gradient", "enable gradient ui", g };
        clib_decl_param(&f_param);
        clib_decl_param(&g_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "-gf"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const Params* result = clib_parse(argc, args);

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
        clib_print_status();
        clib_init();

        Param f_param = {'f', "fullscreen", "toggle fullscreen automatically", f };
        clib_decl_param(&f_param);

        // prepare C arguments, to simulate main(const char** argv, int argc )
        const char* args[]{"executable", "--fullscreen"};
        int argc = sizeof(args) / sizeof(void*);

        // act
        const Params* result = clib_parse(argc, args);

        // test
        EXPECT_TRUE(result->data);
        EXPECT_STREQ(result->data[0].flag_word, f_param.flag_word);
        EXPECT_EQ(result->size, 1);

        // free mem
        clib_shutdown();
    }

}  // namespace
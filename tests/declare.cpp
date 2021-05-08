#include "gtest/gtest.h"
#include "clib.h"

namespace
{
    void f(){ std::cout << "fullscreen on." << std::endl; };

    TEST(declare, a_single_param)
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

}  // namespace
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

    TEST(API, buffer_grow_incrementally)
    {
        clib_init();
        Params buffer;
        clib_buffer_alloc(&buffer, 0); // to be sure first alloc grow_amount will be 1

        size_t grow_amount;
        size_t size = 0;
        for(grow_amount = 1; grow_amount < 465; ++grow_amount)
        {
            size += grow_amount;
            clib_buffer_grow_size(&buffer, grow_amount);
            EXPECT_TRUE( buffer.size <= buffer.capacity );
            EXPECT_TRUE( (buffer.capacity % CLIB_BUF_CHUNK_SIZE) == 0 || buffer.capacity < CLIB_BUF_CHUNK_SIZE );

            // cap should be in  [size, 2 * size[ interval
            EXPECT_EQ(buffer.size, size);
            EXPECT_LE(buffer.size, buffer.capacity);
            EXPECT_GT(buffer.size * 2, buffer.capacity);
        }
        clib_shutdown();
    }

    TEST(API, buffer_single_grow_above_chunk_size)
    {
        size_t size;
        for(size = CLIB_BUF_CHUNK_SIZE; size <= 46087; ++size)
        {
            clib_init();
            Params buffer;
            clib_buffer_alloc(&buffer, 1); // to be sure first alloc size will be 1
            clib_buffer_grow_size(&buffer, size);

            // size should be size and must be close to capacity +/- chunk size.
            EXPECT_EQ(buffer.size, size);
            EXPECT_LE(buffer.size, buffer.capacity);
            EXPECT_GE(buffer.size + CLIB_BUF_CHUNK_SIZE, buffer.capacity);

            EXPECT_EQ(buffer.capacity % CLIB_BUF_CHUNK_SIZE, 0); // must be a multiple of chunk size
            clib_shutdown();
        }
    }

}  // namespace
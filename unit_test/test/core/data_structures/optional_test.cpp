/*******************************************************************************
 * unit_test/test/core/data_structures/optional_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/nullptr.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../common/models/basic_movable_copyable.hpp"
#include "../common/models/special_mem_fun_stats.hpp"

namespace sake_unit_test
{

void optional_test(sake::test::environment& env)
{
    SAKE_USING_NULLPTR;

    {
        sake::optional< int > x;
        SAKE_TEST_CHECK( env, !x.initialized() );
        SAKE_TEST_CHECK( env, !x );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
        x = 1;
        SAKE_TEST_REQUIRE( env, x.initialized() );
        SAKE_TEST_CHECK( env, x );
        SAKE_TEST_REQUIRE_RELATION( env, x.get_ptr(), !=, nullptr );
        SAKE_TEST_CHECK_RELATION( env, *x.get_ptr(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, x.get(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, *x, ==, 1 );
        x = 2;
        SAKE_TEST_REQUIRE( env, x.initialized() );
        SAKE_TEST_REQUIRE_RELATION( env, x.get_ptr(), !=, nullptr );
        SAKE_TEST_CHECK_RELATION( env, *x.get_ptr(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.get(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, *x, ==, 2 );
        x.reset(3);
        SAKE_TEST_REQUIRE( env, x.initialized() );
        SAKE_TEST_REQUIRE_RELATION( env, x.get_ptr(), !=, nullptr );
        SAKE_TEST_CHECK_RELATION( env, *x.get_ptr(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, x.get(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, *x, ==, 3 );
        x.reset();
        SAKE_TEST_CHECK( env, !x.initialized() );
        SAKE_TEST_CHECK( env, !x );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
    }
    {
        typedef models::basic_movable_copyable<> type;
        models::special_mem_fun_stats stats;
        sake::optional< type > x;
        SAKE_TEST_CHECK( env, !x );
        x = type(stats);
        SAKE_TEST_CHECK( env, x );
        x = type(stats);
        SAKE_TEST_CHECK( env, x );
        x.reset(type(stats));
        SAKE_TEST_CHECK( env, x );
        x.reset();
        SAKE_TEST_CHECK( env, !x );
        x.reset(type(stats));
        SAKE_TEST_CHECK( env, x );
        x.reset();
        SAKE_TEST_CHECK( env, !x );
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 7 );
        stats.reset();
        {
            type y(stats);
            x = y;
            x = y;
            x.reset(y);
            x.reset();
            x.reset(y);
            x.reset();
            x = sake::move(y);
            x = sake::move(y);
            x.reset(sake::move(y));
            x.reset();
            x.reset(sake::move(y));
            x.reset();
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 7 );
    }
}

} // namespace sake_unit_test

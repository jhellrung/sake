/*******************************************************************************
 * unit_test/test/core/data_structures/optional_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/data_structures/optional/operators.hpp>
#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/functional/identity.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/zero.hpp>
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
        sake::optional< int > x;
        SAKE_TEST_CHECK( env, !x );
        sake::optional< int > y(x);
        SAKE_TEST_CHECK( env, !y );
        x = 0;
        SAKE_TEST_CHECK( env, x );
        sake::optional< int > z(x);
        SAKE_TEST_REQUIRE( env, z );
        SAKE_TEST_CHECK_RELATION( env, z.get(), ==, 0 );
        y = x;
        SAKE_TEST_CHECK( env, y );
        SAKE_TEST_CHECK_RELATION( env, y.get(), ==, 0 );
        x.reset();
        z = x;
        SAKE_TEST_CHECK( env, !z);
        x = 1;
        y.swap(x);
        SAKE_TEST_REQUIRE( env, x );
        SAKE_TEST_REQUIRE( env, y );
        SAKE_TEST_CHECK_RELATION( env, x.get(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.get(), ==, 1 );
        z.swap(x);
        SAKE_TEST_CHECK( env, !x );
        SAKE_TEST_REQUIRE( env, z );
        SAKE_TEST_CHECK_RELATION( env, z.get(), ==, 0 );
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
    {
        sake::optional< int& > x;
        int i = 0;
        SAKE_TEST_CHECK( env, !x.initialized() );
        SAKE_TEST_CHECK( env, !x );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
        x = i;
        SAKE_TEST_CHECK( env, x.initialized() );
        SAKE_TEST_CHECK( env, x );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), !=, nullptr );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, &i );
        x.reset();
        SAKE_TEST_CHECK( env, !x.initialized() );
        SAKE_TEST_CHECK( env, !x );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
    }
    {
        sake::optional< int& > x;
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
        sake::optional< int& > y(x);
        SAKE_TEST_CHECK_RELATION( env, y.get_ptr(), ==, nullptr );
        int i = 0;
        x = i;
        sake::optional< int& > z(x);
        SAKE_TEST_CHECK_RELATION( env, z.get_ptr(), ==, &i );
        y = x;
        SAKE_TEST_CHECK_RELATION( env, y.get_ptr(), ==, &i );
        x.reset();
        z = x;
        SAKE_TEST_CHECK_RELATION( env, z.get_ptr(), ==, nullptr );
        int j = 1;
        x = j;
        y.swap(x);
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, &i );
        SAKE_TEST_CHECK_RELATION( env, y.get_ptr(), ==, &j );
        z.swap(x);
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
        SAKE_TEST_CHECK_RELATION( env, z.get_ptr(), ==, &i );
    }
    {
        sake::optional< int > x;
        sake::optional< int > y;
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x = 0;
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
        y = 0;
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x = 1;
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
    }
    {
        sake::optional< sake::functional::identity > x;
        SAKE_TEST_CHECK( env, !x );
        sake::optional< int > i = x(0);
        SAKE_TEST_CHECK( env, !i );
        x = sake::identity;
        i = x(0);
        SAKE_TEST_REQUIRE( env, i );
        SAKE_TEST_CHECK_RELATION( env, i.get(), ==, 0 );
    }
}

} // namespace sake_unit_test

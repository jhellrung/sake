/*******************************************************************************
 * unit_test/test/core/data_structures/pair_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/config.hpp>
#include <sake/core/data_structures/pair/construct.hpp>
#include <sake/core/data_structures/pair/operators.hpp>
#include <sake/core/data_structures/pair/pair.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/move.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../common/models/basic_movable_copyable.hpp"
#include "../common/models/special_mem_fun_stats.hpp"

namespace sake_unit_test
{

void pair_test(sake::test::environment& env)
{
    {
        sake::pair< int, int > x(0,1);
        SAKE_TEST_CHECK_RELATION( env, x.first, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.second, ==, 1 );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x.first, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second, ==, 3 );
    }
    {
        sake::pair< int, int > x(0,1);
        sake::pair< int, int > y(x);
        SAKE_TEST_CHECK_RELATION( env, y.first, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.second, ==, 1 );
        y.assign(2,3);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x.first, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second, ==, 3 );
        x.assign(0,1);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x.first, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, y.first, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.second, ==, 1 );
    }
    {
        int i = 1;
        sake::pair< int, int& > x(0,i);
        SAKE_TEST_CHECK_RELATION( env, x.first, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.second, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, &x.second, ==, &i );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x.first, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, 3 );
        i = 4;
        SAKE_TEST_CHECK_RELATION( env, x.second, ==, 4 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
        {
            type y(stats);
            sake::pair< type, type > x(y, sake::move(y));
            x.assign(sake::move(y), y);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 3 );
        stats.reset();
        {
            type y(stats);
            sake::pair< type, type > x(type(stats), y);
            x.assign(y, type(stats));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 5 );
        stats.reset();
        {
            sake::pair< type, type > x((type(stats)), (type(stats)));
            sake::pair< type, type > y(x);
            sake::pair< type, type > z(sake::move(y));
            y = sake::pair< type, type >(type(stats), type(stats));
            z = x;
            x = sake::move(y);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 6 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 12 );
        stats.reset();
        {
            sake::pair< char, type > w(static_cast< char >(0), (type(stats)));
            sake::pair< short, type > x(w);
            sake::pair< int, type > y(sake::move(x));
            sake::pair< long, type > z(
                sake::construct::pair(0, sake::move(x.second)));
            x = w;
            y = sake::move(x);
            z = sake::construct::pair(0, sake::move(x.second));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 6 );
#else // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 5 );
#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 8 );
#else // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 7 );
#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
        stats.reset();
    }
    {
        sake::pair< int, int > x(0,0);
        sake::pair< int, int > y(0,0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x.first = 1;
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
        x.assign(0,1);
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
    }
}

} // namespace sake_unit_test

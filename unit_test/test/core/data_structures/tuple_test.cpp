/*******************************************************************************
 * unit_test/test/core/data_structures/tuple_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/data_structures/tuple/make.hpp>
#include <sake/core/data_structures/tuple/operators.hpp>
#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/emplacer/make.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../common/models/basic_movable_copyable.hpp"
#include "../common/models/special_mem_fun_stats.hpp"

namespace sake_unit_test
{

void tuple_test(sake::test::environment& env)
{
    {
        sake::tuple< int > x(0);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 0 );
        x.assign(1);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 1 );
    }
    {
        sake::tuple< int > x(0);
        sake::tuple< int > y(x);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 0 );
        y.assign(1);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 1 );
        x.assign(0);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, y._0, ==, 0 );
    }
    {
        int i = 0;
        sake::tuple< int& > x(i);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, &x._0, ==, &i );
        x.assign(1);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, 1 );
        i = 2;
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 2 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
        {
            type z(stats);
            sake::tuple< type > w(z);
            sake::tuple< type > x(sake::move(z));
            sake::tuple< type > y((type(stats)));
            w.assign(z);
            x.assign(sake::move(z));
            y.assign(type(stats));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 6 );
        stats.reset();
        {
            sake::tuple< type > x((type(stats)));
            sake::tuple< type > y(x);
            sake::tuple< type > z(sake::move(y));
            y = sake::tuple< type >(type(stats));
            z = x;
            x = sake::move(y);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 6 );
        stats.reset();
    }
    {
        sake::tuple< int > x(0);
        sake::tuple< int > y(0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x._0 = 1;
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
    }



    {
        sake::tuple< int, int > x(0,1);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 1 );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 3 );
    }
    {
        sake::tuple< int, int > x(0,1);
        sake::tuple< int, int > y(x);
        SAKE_TEST_CHECK_RELATION( env, y._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y._1, ==, 1 );
        y.assign(2,3);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 3 );
        x.assign(0,1);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, y._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y._1, ==, 1 );
    }
    {
        int i = 1;
        sake::tuple< int, int& > x(0,i);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, &x._1, ==, &i );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, 3 );
        i = 4;
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 4 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
        {
            type z(stats);
            sake::tuple< type, type > x(z, sake::move(z));
            sake::tuple< type, type > y((type(stats)), z);
            x.assign(sake::move(z), z);
            y.assign(z, type(stats));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 7 );
        stats.reset();
        {
            sake::tuple< type, type > x((type(stats)), (type(stats)));
            sake::tuple< type, type > y(x);
            sake::tuple< type, type > z(sake::move(y));
            y = sake::tuple< type, type >(type(stats), type(stats));
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
            sake::tuple< char, type > w(static_cast< char >(0), (type(stats)));
            sake::tuple< short, type > x(w);
            sake::tuple< int, type > y(sake::move(x));
            sake::tuple< long, type > z(sake::make_tuple(0, sake::move(x._1)));
            x = w;
            y = sake::move(x);
            z = sake::make_tuple(0, sake::move(x._1));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 6 );
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 5 );
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 8 );
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 7 );
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
        stats.reset();
    }
    {
        sake::tuple< int, int > x(0,0);
        sake::tuple< int, int > y(0,0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x._0 = 1;
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



    {
        sake::tuple< int, int, int > x(0,1,2);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, x._2, ==, 2 );
        x.assign(3,4,5);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, x._2, ==, 5 );
    }
    {
        sake::tuple< int, int, int > x(0,1,2);
        sake::tuple< int, int, int > y(x);
        SAKE_TEST_CHECK_RELATION( env, y._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y._1, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, y._2, ==, 2 );
        y.assign(3,4,5);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, x._2, ==, 5 );
        x.assign(0,1,2);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x._0, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, x._1, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, x._2, ==, 5 );
        SAKE_TEST_CHECK_RELATION( env, y._0, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y._1, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, y._2, ==, 2 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
        {
            type y(stats);
            sake::tuple< type, type, type > x(y, sake::move(y), y);
            x.assign(sake::move(y), y, sake::move(y));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 4 );
        stats.reset();
        {
            sake::tuple< type, type, type > x(
                sake::make_emplacer(stats),
                sake::make_emplacer(stats),
                sake::make_emplacer(stats)
            );
            sake::tuple< type, type, type > y(x);
            sake::tuple< type, type, type > z(sake::move(y));
            y = sake::tuple< type, type, type >(
                sake::make_emplacer(stats),
                sake::make_emplacer(stats),
                sake::make_emplacer(stats)
            );
            z = x;
            x = sake::move(y);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 6 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 6 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 12 );
        stats.reset();
    }
    {
        sake::tuple< int, int, int > x(0,0,0);
        sake::tuple< int, int, int > y(0,0,0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x._0 = 1;
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
        x.assign(0,1,0);
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
        x.assign(0,0,1);
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
    }
}

} // namespace sake_unit_test

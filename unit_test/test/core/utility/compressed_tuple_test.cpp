/*******************************************************************************
 * unit_test/test/core/utility/compressed_tuple_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/core/math/cmp.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_cast.hpp>
#include <sake/core/utility/compressed_tuple/compressed_tuple.hpp>
#include <sake/core/utility/compressed_tuple/operators.hpp>
#include <sake/core/utility/emplacer/make.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../common/models/basic_movable_copyable.hpp"
#include "../common/models/special_mem_fun_stats.hpp"

namespace sake_unit_test
{

namespace
{

struct X { };
struct Y { };
struct Z { char _[42]; };

BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< int, X, Y > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, int, Y > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, Y, int > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, Y, Z > ) == sizeof( Z )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, Z, Y > ) == sizeof( Z )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< Z, X, Y > ) == sizeof( Z )));

#ifndef __GNUC__
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, X, X > ) == sizeof( X )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< int, X, X > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, int, X > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, X, int > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, X, Z > ) == sizeof( Z )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, Z, X > ) == sizeof( Z )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< Z, X, X > ) == sizeof( Z )));
#endif // #ifndef __GNUC__

} // namespace

void compressed_tuple_test(sake::test::environment& env)
{
    {
        sake::compressed_tuple< int > x(0);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        x.assign(1);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 1 );
    }
    {
        sake::compressed_tuple< int > x(0);
        sake::compressed_tuple< int > y(x);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        y.assign(1);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 1 );
        x.assign(0);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<0>(), ==, 0 );
    }
    {
        int i = 0;
        sake::compressed_tuple< int& > x(i);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, &x.at_c<0>(), ==, &i );
        x.assign(1);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, 1 );
        i = 2;
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 2 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
#ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_STATIC_ASSERT((!sake::has_move_emulation<
           sake::compressed_tuple< int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< type > >::value));
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
        {
            type z(stats);
            sake::compressed_tuple< type > w(z);
            sake::compressed_tuple< type > x(sake::move(z));
            sake::compressed_tuple< type > y(SAKE_RV_CAST(type(stats)));
            w.assign(z);
            x.assign(sake::move(z));
            y.assign(SAKE_RV_CAST(type(stats)));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 6 );
        stats.reset();
        {
            sake::compressed_tuple< type > x(SAKE_RV_CAST(type(stats)));
            sake::compressed_tuple< type > y(x);
            sake::compressed_tuple< type > z(sake::move(y));
            y = SAKE_RV_CAST(sake::compressed_tuple< type >(SAKE_RV_CAST(type(stats))));
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
        sake::compressed_tuple< int > x(0);
        sake::compressed_tuple< int > y(0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x.at_c<0>() = 1;
        SAKE_TEST_CHECK_ALL( env, (!(x == y)) (!(x <= y)) ( (x >= y))
                                  ( (x != y)) (!(x <  y)) ( (x >  y))
                                  (!(y == x)) ( (y <= x)) (!(y >= x))
                                  ( (y != x)) ( (y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), >, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), <, sake::zero );
    }



    {
        sake::compressed_tuple< int, int > x(0,1);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 1 );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 3 );
    }
    {
        sake::compressed_tuple< int, int > x(0,1);
        sake::compressed_tuple< int, int > y(x);
        SAKE_TEST_CHECK_RELATION( env, y.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<1>(), ==, 1 );
        y.assign(2,3);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 3 );
        x.assign(0,1);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<1>(), ==, 1 );
    }
    {
        int i = 1;
        sake::compressed_tuple< int, int& > x(0,i);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, &x.at_c<1>(), ==, &i );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, 3 );
        i = 4;
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 4 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
#ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_STATIC_ASSERT((!sake::has_move_emulation<
           sake::compressed_tuple< int, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< int, type > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< type, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< type, type > >::value));
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
        {
            type z(stats);
            sake::compressed_tuple< type, type > x(z, sake::move(z));
            sake::compressed_tuple< type, type > y(SAKE_RV_CAST(type(stats)), z);
            x.assign(sake::move(z), z);
            y.assign(z, SAKE_RV_CAST(type(stats)));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 7 );
        stats.reset();
        {
            sake::compressed_tuple< type, type > x(
                SAKE_RV_CAST(type(stats)),
                SAKE_RV_CAST(type(stats))
            );
            sake::compressed_tuple< type, type > y(x);
            sake::compressed_tuple< type, type > z(sake::move(y));
            y = SAKE_RV_CAST((sake::compressed_tuple< type, type >(
                sake::make_emplacer(stats),
                sake::make_emplacer(stats)
            )));
            z = x;
            x = sake::move(y);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 10 );
        stats.reset();
        {
            sake::compressed_tuple< char, type > w(
                static_cast< char >(0),
                SAKE_RV_CAST(type(stats))
            );
            sake::compressed_tuple< short, type > x(w);
            sake::compressed_tuple< int, type > y(sake::move(x));
            x = w;
            y = sake::move(x);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 4 );
        stats.reset();
    }
    {
        sake::compressed_tuple< int, int > x(0,0);
        sake::compressed_tuple< int, int > y(0,0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x.at_c<0>() = 1;
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
        sake::compressed_tuple< int, int, int > x(0,1,2);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<2>(), ==, 2 );
        x.assign(3,4,5);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<2>(), ==, 5 );
    }
    {
        sake::compressed_tuple< int, int, int > x(0,1,2);
        sake::compressed_tuple< int, int, int > y(x);
        SAKE_TEST_CHECK_RELATION( env, y.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<1>(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<2>(), ==, 2 );
        y.assign(3,4,5);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<2>(), ==, 5 );
        x.assign(0,1,2);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<2>(), ==, 5 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<1>(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, y.at_c<2>(), ==, 2 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
#ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_STATIC_ASSERT((!sake::has_move_emulation<
           sake::compressed_tuple< int, int, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< int, int, type > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< int, type, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< int, type, type > >::value));
        BOOST_STATIC_ASSERT((!sake::has_move_emulation<
           sake::compressed_tuple< type, int, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< type, int, type > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< type, type, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_tuple< type, type, type > >::value));
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
        {
            type y(stats);
            sake::compressed_tuple< type, type, type > x(y, sake::move(y), y);
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
            sake::compressed_tuple< type, type, type > x(
                sake::make_emplacer(stats),
                sake::make_emplacer(stats),
                sake::make_emplacer(stats)
            );
            sake::compressed_tuple< type, type, type > y(x);
            sake::compressed_tuple< type, type, type > z(sake::move(y));
            y = SAKE_RV_CAST((sake::compressed_tuple< type, type, type >(
                sake::make_emplacer(stats),
                sake::make_emplacer(stats),
                sake::make_emplacer(stats)
            )));
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
        sake::compressed_tuple< int, int, int > x(0,0,0);
        sake::compressed_tuple< int, int, int > y(0,0,0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x.at_c<0>() = 1;
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


    {
        sake::compressed_tuple< int, X > x0(0,(X()));
        SAKE_TEST_CHECK_RELATION( env, x0.at_c<0>(), ==, 0 );
        x0.at_c<0>() = 1;
        SAKE_TEST_CHECK_RELATION( env, x0.at_c<0>(), ==, 1 );
        sake::compressed_tuple< X, int > x1((X()),0);
        SAKE_TEST_CHECK_RELATION( env, x1.at_c<1>(), ==, 0 );
        x1.at_c<1>() = 1;
        SAKE_TEST_CHECK_RELATION( env, x1.at_c<1>(), ==, 1 );
    }
    {
        sake::compressed_tuple< int, X, X > x0(0,(X()),(X()));
        SAKE_TEST_CHECK_RELATION( env, x0.at_c<0>(), ==, 0 );
        x0.at_c<0>() = 1;
        SAKE_TEST_CHECK_RELATION( env, x0.at_c<0>(), ==, 1 );
        sake::compressed_tuple< X, int, X > x1((X()),0,(X()));
        SAKE_TEST_CHECK_RELATION( env, x1.at_c<1>(), ==, 0 );
        x1.at_c<1>() = 1;
        SAKE_TEST_CHECK_RELATION( env, x1.at_c<1>(), ==, 1 );
        sake::compressed_tuple< X, X, int > x2((X()),(X()),0);
        SAKE_TEST_CHECK_RELATION( env, x2.at_c<2>(), ==, 0 );
        x2.at_c<2>() = 1;
        SAKE_TEST_CHECK_RELATION( env, x2.at_c<2>(), ==, 1 );
    }
}

} // namespace sake_unit_test

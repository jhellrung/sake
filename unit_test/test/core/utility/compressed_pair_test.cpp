/*******************************************************************************
 * unit_test/test/core/utility/compressed_pair_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/compressed_pair/operators.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/move.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../common/models/basic_movable_copyable.hpp"
#include "../common/models/special_mem_fun_stats.hpp"

namespace sake_unit_test
{

namespace
{

struct X { };

struct Y { char _[42]; };

BOOST_STATIC_ASSERT((sizeof( sake::compressed_pair< int, X > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_pair< X, int > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_pair< X, Y > ) == sizeof( Y )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_pair< Y, X > ) == sizeof( Y )));

#ifndef __GNUC__
BOOST_STATIC_ASSERT((sizeof( sake::compressed_pair< X, X > ) == sizeof( X )));
#endif // #ifndef __GNUC__

} // namespace

void compressed_pair_test(sake::test::environment& env)
{
    {
        sake::compressed_pair< int, int > x(0,1);
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 1 );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 3 );
    }
    {
        sake::compressed_pair< int, int > x(0,1);
        sake::compressed_pair< int, int > y(x);
        SAKE_TEST_CHECK_RELATION( env, y.first(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.second(), ==, 1 );
        y.assign(2,3);
        x = y;
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 3 );
        x.assign(0,1);
        x.swap(y);
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, y.first(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.second(), ==, 1 );
    }
    {
        int i = 1;
        sake::compressed_pair< int, int& > x(0,i);
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, &x.second(), ==, &i );
        x.assign(2,3);
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, 3 );
        i = 4;
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 4 );
    }
    {
        models::special_mem_fun_stats stats;
        typedef models::basic_movable_copyable<> type;
#ifdef BOOST_NO_RVALUE_REFERENCES
        BOOST_STATIC_ASSERT((!sake::has_move_emulation<
           sake::compressed_pair< int, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_pair< int, type > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_pair< type, int > >::value));
        BOOST_STATIC_ASSERT((sake::has_move_emulation<
           sake::compressed_pair< type, type > >::value));
#endif // #ifdef BOOST_NO_RVALUE_REFERENCES
        {
            type y(stats);
            sake::compressed_pair< type, type > x(y, sake::move(y));
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
            sake::compressed_pair< type, type > x(type(stats), y);
            x.assign(y, type(stats));
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 5 );
#ifndef BOOST_NO_RVALUE_REFERENCES
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 1 );
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 0 );
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
        stats.reset();
        {
            sake::compressed_pair< type, type > x((type(stats)), (type(stats)));
            sake::compressed_pair< type, type > y(x);
            sake::compressed_pair< type, type > z(sake::move(y));
            y = sake::compressed_pair< type, type >(type(stats), type(stats));
            z = x;
            x = sake::move(y);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 12 );
#ifndef BOOST_NO_RVALUE_REFERENCES
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 6 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 4 );
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 6 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 4 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
        stats.reset();
    }
    {
        sake::compressed_pair< int, int > x(0,0);
        sake::compressed_pair< int, int > y(0,0);
        SAKE_TEST_CHECK_ALL( env, ( (x == y)) ( (x <= y)) ( (x >= y))
                                  (!(x != y)) (!(x <  y)) (!(x >  y))
                                  ( (y == x)) ( (y <= x)) ( (y >= x))
                                  (!(y != x)) (!(y <  x)) (!(y >  x)) );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(x,y), ==, sake::zero );
        SAKE_TEST_CHECK_RELATION( env, sake::cmp(y,x), ==, sake::zero );
        x.first() = 1;
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

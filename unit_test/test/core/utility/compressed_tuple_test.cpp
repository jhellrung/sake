/*******************************************************************************
 * unit_test/test/core/utility/compressed_tuple_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>

#include <sake/core/utility/compressed_tuple.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

struct X { };

struct Y { char _[42]; };

BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< int, X, X > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, int, X > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, X, int > ) == sizeof( int )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, X, Y > ) == sizeof( Y )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< X, Y, X > ) == sizeof( Y )));
BOOST_STATIC_ASSERT((sizeof( sake::compressed_tuple< Y, X, X > ) == sizeof( Y )));

} // namespace

void compressed_tuple_test(sake::test::environment& env)
{
    {
        sake::compressed_tuple< int, int, int > x(0,1,2);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<1>(), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<2>(), ==, 2 );
    }
    {
        sake::compressed_tuple< int, X, int > x(0, X(), 2);
        SAKE_TEST_CHECK_RELATION( env, x.at_c<0>(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.at_c<2>(), ==, 2 );
    }
}

} // namespace sake_unit_test

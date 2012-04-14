/*******************************************************************************
 * unit_test/test/core/utility/compressed_pair_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>

#include <sake/core/utility/compressed_pair/compressed_pair.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

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

} // namespace

void compressed_pair_test(sake::test::environment& env)
{
    {
        sake::compressed_pair< int, int > x(0,1);
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 1 );
    }
    {
        sake::compressed_pair< int, X > x(0, X());
        SAKE_TEST_CHECK_RELATION( env, x.first(), ==, 0 );
    }
    {
        sake::compressed_pair< X, int > x(X(), 1);
        SAKE_TEST_CHECK_RELATION( env, x.second(), ==, 1 );
    }
}

} // namespace sake_unit_test

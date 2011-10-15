/*******************************************************************************
 * unit_test/test/core/math/compare_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/compare.hpp>
#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/sign_t.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

template< int >
struct X
{
    int x;
    explicit X(int const x_) : x(x_) { }
};

inline int
compare(X<0> const x0, X<0> const x1)
{ return sake::compare(x0.x, x1.x).as_int(); }

inline sake::fuzzy_sign_t
compare(X<0> const x0, X<1> const x1)
{ return sake::compare(x0.x, x1.x); }

struct Y
{
    int x;
    explicit Y(int const x_) : x(x_) { }
    sake::sign_t compare(Y const other) const
    { return sake::compare(x, other.x); }
    sake::sign_t compare(int const other) const
    { return sake::compare(x, other); }
};

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::compare< int, int >::type,
    sake::sign_t
>::value));

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::compare< X<0>, X<0> >::type,
    sake::sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::compare< X<0>, X<1> >::type,
    sake::fuzzy_sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::compare< X<1>, X<0> >::type,
    sake::fuzzy_sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::compare<Y,Y>::type,
    sake::sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::compare< Y, int >::type,
    sake::sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::compare< int, Y >::type,
    sake::sign_t
>::value));

} // namespace

void compare_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::compare(-1,-1).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(-1, 0).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(-1,+1).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare( 0,-1).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare( 0, 0).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare( 0,+1).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(+1,-1).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(+1, 0).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(+1,+1).as_int(), ==,  0 );

    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<0>(0), X<0>(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<0>(0), X<0>( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<0>(0), X<0>(+1)).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<0>(0), X<1>(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<0>(0), X<1>( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<0>(0), X<1>(+1)).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<1>(0), X<0>(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<1>(0), X<0>( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(X<1>(0), X<0>(+1)).as_int(), ==, -1 );

    SAKE_TEST_CHECK_RELATION( env, sake::compare(Y(0),-1).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(Y(0), 0).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(Y(0),+1).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(0,Y(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(0,Y( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::compare(0,Y(+1)).as_int(), ==, -1 );
}

} // namespace sake_unit_test

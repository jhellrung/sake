/*******************************************************************************
 * unit_test/test/core/math/cmp_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/cmp.hpp>
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
cmp(X<0> const x0, X<0> const x1)
{ return sake::cmp(x0.x, x1.x).as_int(); }

inline sake::fuzzy_sign_t
cmp(X<0> const x0, X<1> const x1)
{ return sake::cmp(x0.x, x1.x); }

struct Y
{
    int x;
    explicit Y(int const x_) : x(x_) { }
    sake::sign_t cmp(Y const other) const
    { return sake::cmp(x, other.x); }
    sake::sign_t cmp(int const other) const
    { return sake::cmp(x, other); }
};

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::cmp< int, int >::type,
    sake::sign_t
>::value));

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::cmp< X<0>, X<0> >::type,
    sake::sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::cmp< X<0>, X<1> >::type,
    sake::fuzzy_sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::cmp< X<1>, X<0> >::type,
    sake::fuzzy_sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::cmp<Y,Y>::type,
    sake::sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::cmp< Y, int >::type,
    sake::sign_t
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::cmp< int, Y >::type,
    sake::sign_t
>::value));

} // namespace

void cmp_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(-1,-1).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(-1, 0).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(-1,+1).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp( 0,-1).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp( 0, 0).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp( 0,+1).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(+1,-1).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(+1, 0).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(+1,+1).as_int(), ==,  0 );

    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<0>(0), X<0>(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<0>(0), X<0>( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<0>(0), X<0>(+1)).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<0>(0), X<1>(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<0>(0), X<1>( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<0>(0), X<1>(+1)).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<1>(0), X<0>(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<1>(0), X<0>( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(X<1>(0), X<0>(+1)).as_int(), ==, -1 );

    SAKE_TEST_CHECK_RELATION( env, sake::cmp(Y(0),-1).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(Y(0), 0).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(Y(0),+1).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(0,Y(-1)).as_int(), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(0,Y( 0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::cmp(0,Y(+1)).as_int(), ==, -1 );
}

} // namespace sake_unit_test

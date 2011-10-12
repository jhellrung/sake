/*******************************************************************************
 * unit_test/test/core/math/sign_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/sign.hpp>
#include <sake/core/math/sign_t.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

struct X
{
    int x;
    explicit X(int const x_) : x(x_) { }
};

inline int
sign(X const x)
{ return sake::sign(x.x).as_int(); }

struct Y
{
    int x;
    explicit Y(int const x_) : x(x_) { }
    sake::fuzzy_sign_t sign() const
    { return sake::sign(x); }
};

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::sign< int >::type,
    sake::sign_t
>::value));

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::sign<X>::type,
    sake::sign_t
>::value));

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::sign<Y>::type,
    sake::fuzzy_sign_t
>::value));

} // namespace

void sign_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::sign(-10).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::sign(  0).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::sign(+10).as_int(), ==, +1 );

    SAKE_TEST_CHECK_RELATION( env, sake::sign(X(-10)).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::sign(X(  0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::sign(X(+10)).as_int(), ==, +1 );

    SAKE_TEST_CHECK_RELATION( env, sake::sign(Y(-10)).as_int(), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::sign(Y(  0)).as_int(), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::sign(Y(+10)).as_int(), ==, +1 );
}

} // namespace sake_unit_test

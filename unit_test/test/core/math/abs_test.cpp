/*******************************************************************************
 * unit_test/test/core/math/abs_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/math/abs.hpp>
#include <sake/core/math/zero.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

#if 0
struct X
{
    int x;
    explicit X(int const x_) : x(x_) { }
};

inline X
abs(X const x)
{ return X(static_cast< int >(sake::abs(x))); }

struct Y
{
    int x;
    explicit Y(int const x_) : x(x_) { }
    Y abs() const
    { return Y(static_cast< int >(sake::abs(x))); }
};

struct Z
{
    int x;
    explicit Z(int const x_) : x(x_) { }
    Z operator-() const
    { return Z(-x); }
};

bool operator<(Z const x, sake::zero_t)
{ return x.x < 0; }

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::abs< int >::type,
    unsigned int
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::abs< unsigned int >::type,
    unsigned int
>::value));

BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::abs< float >::type,
    float
>::value));
BOOST_STATIC_ASSERT((boost::is_same<
    sake::result_of::abs< double >::type,
    double
>::value));
#endif

} // namespace

void abs_test(sake::test::environment& env)
{
#if 0
    SAKE_TEST_CHECK_RELATION( env, sake::abs(-10), ==, +10 );
    SAKE_TEST_CHECK_RELATION( env, sake::abs(+10), ==, +10 );

    SAKE_TEST_CHECK_RELATION( env, sake::abs(-10.0), ==, +10.0 );
    SAKE_TEST_CHECK_RELATION( env, sake::abs(+10.0), ==, +10.0 );

    SAKE_TEST_CHECK_RELATION( env, sake::abs(X(-10)).x, ==, +10 );
    SAKE_TEST_CHECK_RELATION( env, sake::abs(X(+10)).x, ==, +10 );

    SAKE_TEST_CHECK_RELATION( env, sake::abs(Y(-10)).x, ==, +10 );
    SAKE_TEST_CHECK_RELATION( env, sake::abs(Y(+10)).x, ==, +10 );

    SAKE_TEST_CHECK_RELATION( env, sake::abs(Z(-10)).x, ==, +10 );
    SAKE_TEST_CHECK_RELATION( env, sake::abs(Z(+10)).x, ==, +10 );
#endif
}

} // namespace sake_unit_test

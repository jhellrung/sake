/*******************************************************************************
 * unit_test/test/core/utility/as_reference_to_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/utility/as_reference_to.hpp>
#include <sake/core/utility/type_tag.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

struct A
{
    int& x;
    operator int&() const
    { return x; }
};

int& as_reference_to(A a, sake::type_tag<int>)
{ return a; }

struct B
{
    int& x;
    int& operator*() const
    { return x; }
};

struct C
{
    int& x;
    A operator*() const
    {
        A a = { x };
        return a;
    }
};

} // namespace

void as_reference_to_test(sake::test::environment& env)
{
    int x = 0;
    BOOST_MPL_ASSERT((boost::is_same< int&, sake::result_of::as_reference_to< int&, int >::type >));
    SAKE_TEST_CHECK_RELATION( env, &x, ==, &sake::as_reference_to< int >(x) );
    int* p = &x;
    BOOST_MPL_ASSERT((boost::is_same< int&, sake::result_of::as_reference_to< int*&, int >::type >));
    SAKE_TEST_CHECK_RELATION( env, &x, ==, &sake::as_reference_to< int >(p) );
    int** q = &p;
    BOOST_MPL_ASSERT((boost::is_same< int&, sake::result_of::as_reference_to< int**&, int >::type >));
    SAKE_TEST_CHECK_RELATION( env, &x, ==, &sake::as_reference_to< int >(q) );
    A a = { x }; 
    BOOST_MPL_ASSERT((boost::is_same< int&, sake::result_of::as_reference_to< A&, int >::type >));
    SAKE_TEST_CHECK_RELATION( env, &x, ==, &sake::as_reference_to< int >(a) );
    B b = { x };
    BOOST_MPL_ASSERT((boost::is_same< int&, sake::result_of::as_reference_to< B&, int >::type >));
    SAKE_TEST_CHECK_RELATION( env, &x, ==, &sake::as_reference_to< int >(b) );
    C c = { x };
    BOOST_MPL_ASSERT((boost::is_same< int&, sake::result_of::as_reference_to< C&, int >::type >));
    SAKE_TEST_CHECK_RELATION( env, &x, ==, &sake::as_reference_to< int >(c) );
}

} // namespace sake_unit_test

/*******************************************************************************
 * unit_test/test/core/utility/swap_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/utility/swap.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

template< class T >
struct X
{ T x; };

namespace std_
{

struct X { int x; };

template< class T >
void swap(T&, T&)
{ }

} // namespace std_

namespace boost_
{

struct X { int x; };

template< class T0, class T1 >
void swap(T0&, T1&)
{ }

} // namespace boost_

} // namespace

void swap_test(sake::test::environment& env)
{
    {
        int x = 0, y = 1;
        sake::swap(x, y);
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x, ==, 1 )) (( y, ==, 0 )) );
    }
    {
        X<int> x = { 0 }, y = { 1 };
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
        sake::swap(x, y);
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 1 )) (( y.x, ==, 0 )) );
    }
    {
        std_::X x = { 0 }, y = { 1 };
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
        // sake::swap should *not* swap x and y due to the presence of
        // std_::swap.
        sake::swap(x, y);
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
    }
    {
        boost_::X x = { 0 }, y = { 1 };
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
        // sake::swap should *not* swap x and y due to the presence of
        // boost_::swap.
        sake::swap(x, y);
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
    }
}

} // namespace sake_unit_test

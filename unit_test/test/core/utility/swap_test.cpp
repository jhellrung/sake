/*******************************************************************************
 * unit_test/test/core/utility/swap_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <algorithm>

#include <boost/utility/swap.hpp>

#include <sake/core/utility/swap.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace boost
{

namespace
{

struct X { int x; };

} // namespace

} // namespace boost

namespace sake
{

namespace
{

struct X { int x; };

} // namespace

} // namespace sake

namespace sake_unit_test
{

namespace
{

struct X { int x; };

namespace std_
{

struct X { int x; };

// Define an ADL-accessible swap overload with identical signature to std::swap.
// This should be preferred over sake::swap's default implementation.
template< class T >
void swap(T&, T&)
{ }

} // namespace std_

namespace boost_
{

struct X { int x; };

// Define an ADL-accessible swap overload with identical signature to
// boost::swap.
// This should be preferred over sake::swap's default implementation.
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
        X x = { 0 }, y = { 1 };
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
        sake::swap(x, y);
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 1 )) (( y.x, ==, 0 )) );
    }
    {
        boost::X x = { 0 }, y = { 1 };
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
        // sake::swap should just call boost::swap.
        sake::swap(x, y);
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 1 )) (( y.x, ==, 0 )) );
    }
    {
        sake::X x = { 0 }, y = { 1 };
        SAKE_TEST_CHECK_RELATION_ALL( env, (( x.x, ==, 0 )) (( y.x, ==, 1 )) );
        // sake::swap should just use the default swap implementation and, in
        // particular, it should *not* be confused by the presence of the
        // sake::swap static function object.
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

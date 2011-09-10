/*******************************************************************************
 * unit_test/core/utility/swap_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <algorithm>

#include <boost/swap.hpp>

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
}

} // namespace sake_unit_test

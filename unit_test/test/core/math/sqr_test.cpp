/*******************************************************************************
 * unit_test/test/core/math/sqr_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/sqr.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void sqr_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::sqr(0), ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, sake::sqr(1), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::sqr(2), ==, 4 );
    SAKE_TEST_CHECK_RELATION( env, sake::sqr(3), ==, 9 );
    SAKE_TEST_CHECK_RELATION( env, sake::sqr(-1), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::sqr(-2), ==, 4 );
    SAKE_TEST_CHECK_RELATION( env, sake::sqr(-3), ==, 9 );
}

} // namespace sake_unit_test

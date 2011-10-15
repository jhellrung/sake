/*******************************************************************************
 * unit_test/test/core/math/negate_ip_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/negate_ip.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void negate_ip_test(sake::test::environment& env)
{
    int x = -10;
    SAKE_TEST_CHECK_RELATION( env, &sake::negate_ip(x), ==, &x );
    SAKE_TEST_CHECK_RELATION( env, x, ==, +10 );
    SAKE_TEST_CHECK_RELATION( env, &sake::negate_ip(x), ==, &x );
    SAKE_TEST_CHECK_RELATION( env, x, ==, -10 );
}

} // namespace sake_unit_test

/*******************************************************************************
 * unit_test/test/core/math/clamp_ip_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/clamp_ip.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void clamp_ip_test(sake::test::environment& env)
{
    int i = 0;
    SAKE_TEST_CHECK_RELATION( env, &sake::clamp_ip(-1,i,+1), ==, &i );
    SAKE_TEST_CHECK_RELATION( env, i, ==, 0 );
    i = +2;
    SAKE_TEST_CHECK_RELATION( env, &sake::clamp_ip(-1,i,+1), ==, &i );
    SAKE_TEST_CHECK_RELATION( env, i, ==, +1 );
    i = -2;
    SAKE_TEST_CHECK_RELATION( env, &sake::clamp_ip(-1,i,+1), ==, &i );
    SAKE_TEST_CHECK_RELATION( env, i, ==, -1 );
}

} // namespace sake_unit_test

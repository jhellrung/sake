/*******************************************************************************
 * unit_test/test/core/math/max_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/max.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void max_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::max(0,1), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::max(1,0), ==, 1 );
}

} // namespace sake_unit_test

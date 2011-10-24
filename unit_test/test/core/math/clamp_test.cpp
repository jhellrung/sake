/*******************************************************************************
 * unit_test/test/core/math/clamp_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/clamp.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void clamp_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::clamp(-1, 0,+1), ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, sake::clamp(-1,+1, 0), ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, sake::clamp( 0,-1,+1), ==, 0 );
}

} // namespace sake_unit_test

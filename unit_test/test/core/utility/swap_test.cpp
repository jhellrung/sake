/*******************************************************************************
 * unit_test/core/utility/swap_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <cassert>

#include <algorithm>

#include <sake/core/utility/swap.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void swap_test(sake::test::environment& env)
{
    {
        int x = 0, y = 1;
        sake::swap(x, y);
        SAKE_TEST_CHECK_RELATION( env, x, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, y, ==, 0 );
    }
}

} // namespace sake_unit_test

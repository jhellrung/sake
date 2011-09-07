/*******************************************************************************
 * unit_test/core/utility/utility_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void swap_test(sake::test::environment& env);

void utility_test(sake::test::environment& env)
{
    env("swap", &swap_test);
}

} // namespace sake_unit_test

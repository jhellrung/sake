/*******************************************************************************
 * unit_test/test/core/utility/utility_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/utility/as_reference_to.hpp>
#include <sake/core/utility/identity_type.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void address_of_test(sake::test::environment& env);
void assert_test(sake::test::environment& env);
void swap_test(sake::test::environment& env);

void utility_test(sake::test::environment& env)
{
    env("address_of", &address_of_test);
    env("assert", &assert_test);
    env("swap", &swap_test);
}

} // namespace sake_unit_test

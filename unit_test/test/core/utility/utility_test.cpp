/*******************************************************************************
 * unit_test/test/core/utility/utility_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/utility/identity_type.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void address_of_test(sake::test::environment& env);
void as_reference_to_test(sake::test::environment& env);
void assert_test(sake::test::environment& env);
void compressed_pair_test(sake::test::environment& env);
void compressed_tuple_test(sake::test::environment& env);
void declare_once_test(sake::test::environment& env);
void is_template_base_of_test(sake::test::environment& env);
void overload_test(sake::test::environment& env);
void swap_test(sake::test::environment& env);

void utility_test(sake::test::environment& env)
{
    env("address_of", &address_of_test);
    env("as_reference_to", &as_reference_to_test);
    env("assert", &assert_test);
    env("compressed_pair", &compressed_pair_test);
    env("compressed_tuple", &compressed_tuple_test);
    env("declare_once", &declare_once_test);
    env("is_template_base_of", &is_template_base_of_test);
    env("overload", &overload_test);
    env("swap", &swap_test);
}

} // namespace sake_unit_test

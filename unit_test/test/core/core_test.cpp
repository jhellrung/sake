/*******************************************************************************
 * unit_test/test/core/core_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void expr_traits_test(sake::test::environment& env);
void introspection_test(sake::test::environment& env);
void keyword_test(sake::test::environment& env);
void math_test(sake::test::environment& env);
void ref_test(sake::test::environment& env);
void utility_test(sake::test::environment& env);

void core_test(sake::test::environment& env)
{
    env("expr_traits", &expr_traits_test);
    env("introspection", &introspection_test);
    env("keyword", &keyword_test);
    env("math", &math_test);
    env("ref", &ref_test);
    env("utility", &utility_test);
}

} // namespace sake_unit_test

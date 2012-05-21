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

void data_structures_test(sake::test::environment& env);
void emplacer_test(sake::test::environment& env);
void expr_traits_test(sake::test::environment& env);
void introspection_test(sake::test::environment& env);
void keyword_test(sake::test::environment& env);
void math_test(sake::test::environment& env);
void memory_test(sake::test::environment& env);
void move_test(sake::test::environment& env);
void utility_test(sake::test::environment& env);

void core_test(sake::test::environment& env)
{
    env("data_structures", &data_structures_test);
    env("emplacer", &emplacer_test);
    env("expr_traits", &expr_traits_test);
    env("introspection", &introspection_test);
    env("keyword", &keyword_test);
    env("math", &math_test);
    env("memory", &memory_test);
    env("move", &move_test);
    env("utility", &utility_test);
}

} // namespace sake_unit_test

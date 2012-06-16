/*******************************************************************************
 * unit_test/test/core/iterator/iterator_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace iterator
{

void adaptor_test(sake::test::environment& env);
void archetypes_test(sake::test::environment& env);
void facade_test(sake::test::environment& env);
void introversal_adaptor_test(sake::test::environment& env);

} // namespace iterator

void iterator_test(sake::test::environment& env)
{
    env("adaptor", &iterator::adaptor_test);
    env("archetypes", &iterator::archetypes_test);
    env("facade", &iterator::facade_test);
    env("introversal_adapor", &iterator::introversal_adaptor_test);
}

} // namespace sake_unit_test

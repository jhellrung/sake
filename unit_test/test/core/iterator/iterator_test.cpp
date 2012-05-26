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

void iterator_adaptor_test(sake::test::environment& env);
void iterator_archetypes_test(sake::test::environment& env);
void iterator_facade_test(sake::test::environment& env);

void iterator_test(sake::test::environment& env)
{
    env("iterator_adaptor", &iterator_adaptor_test);
    env("iterator_archetypes", &iterator_archetypes_test);
    env("iterator_facade", &iterator_facade_test);
}

} // namespace sake_unit_test

/*******************************************************************************
 * unit_test/test/core/range/range_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace range
{

//void adaptor_test(sake::test::environment& env);
void archetypes_test(sake::test::environment& env);

} // namespace range

void range_test(sake::test::environment& env)
{
    //env("adaptor", &range::adaptor_test);
    env("archetypes", &range::archetypes_test);
}

} // namespace sake_unit_test

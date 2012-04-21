/*******************************************************************************
 * unit_test/test/core/data_structures/data_structures_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void optional_test(sake::test::environment& env);

void data_structures_test(sake::test::environment& env)
{
    env("optional", &optional_test);
}

} // namespace sake_unit_test

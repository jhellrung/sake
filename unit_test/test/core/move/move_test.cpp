/*******************************************************************************
 * unit_test/test/core/move/move_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void movable_test(sake::test::environment& env);
void rv_sink_test(sake::test::environment& env);

void move_test(sake::test::environment& env)
{
    env("movable", &movable_test);
    env("rv_sink", &rv_sink_test);
}

} // namespace sake_unit_test

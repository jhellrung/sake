/*******************************************************************************
 * unit_test/test/core/range/adaptors/adaptors_test.cpp
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

namespace adaptors
{

void move_test(sake::test::environment& env);
void transform_test(sake::test::environment& env);

} // namespace adaptors

void adaptors_test(sake::test::environment& env)
{
    env("move", &adaptors::move_test);
    env("transform", &adaptors::transform_test);
}

} // namespace range

} // namespace sake_unit_test

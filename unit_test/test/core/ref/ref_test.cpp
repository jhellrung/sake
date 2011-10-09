/*******************************************************************************
 * unit_test/test/core/ref/ref_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void reference_wrapper_test(sake::test::environment& env);

void ref_test(sake::test::environment& env)
{
    env("reference_wrapper", &reference_wrapper_test);
}

} // namespace sake_unit_test

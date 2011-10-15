/*******************************************************************************
 * unit_test/test/core/math/math_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void abs_ip_test(sake::test::environment& env);
void abs_test(sake::test::environment& env);
void compare_test(sake::test::environment& env);
void intlog2_test(sake::test::environment& env);
void negate_ip_test(sake::test::environment& env);
void sign_test(sake::test::environment& env);

void math_test(sake::test::environment& env)
{
    env("abs", &abs_test);
    env("abs_ip", &abs_ip_test);
    env("compare", &compare_test);
    env("intlog2", &intlog2_test);
    env("negate_ip", &negate_ip_test);
    env("sign", &sign_test);
}

} // namespace sake_unit_test

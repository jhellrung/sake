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

void abs_test(sake::test::environment& env);
void abs_ip_test(sake::test::environment& env);
void binom_test(sake::test::environment& env);
void clamp_test(sake::test::environment& env);
void clamp_ip_test(sake::test::environment& env);
void cmp_test(sake::test::environment& env);
void intlog2_test(sake::test::environment& env);
void max_test(sake::test::environment& env);
void min_test(sake::test::environment& env);
void neg_ip_test(sake::test::environment& env);
void pow_test(sake::test::environment& env);
void sign_test(sake::test::environment& env);
void sqr_test(sake::test::environment& env);

void math_test(sake::test::environment& env)
{
    env("abs", &abs_test);
    env("abs_ip", &abs_ip_test);
    env("binom", &binom_test);
    env("clamp", &clamp_test);
    env("clamp_ip", &clamp_ip_test);
    env("cmp", &cmp_test);
    env("intlog2", &intlog2_test);
    env("max", &max_test);
    env("min", &min_test);
    env("neg_ip", &neg_ip_test);
    env("pow", &pow_test);
    env("sign", &sign_test);
    env("sqr", &sqr_test);
}

} // namespace sake_unit_test

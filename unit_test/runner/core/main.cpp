/*******************************************************************************
 * unit_test/runner/core/main.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

void core_test(sake::test::environment& env);

} // namespace sake_unit_test

int main(int argc, char* argv[])
{
    sake::test::environment env;
    env.parse_command_line(argc, argv);
    env("core", &sake_unit_test::core_test);
    env.report();
    return env.main_return_value();
}

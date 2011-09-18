/*******************************************************************************
 * unit_test/test/core/utility/declare_once_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/utility/declare_once.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace
{

SAKE_DECLARE_ONCE_GLOBAL_DECLARATION( xxx );

} // namespace

namespace sake_unit_test
{

void declare_once_test(sake::test::environment& env)
{
    SAKE_DECLARE_ONCE( int, xxx );
    xxx.value = 42;
    SAKE_DECLARE_ONCE( int, xxx );
    SAKE_TEST_CHECK_RELATION( env, xxx.value, ==, 42 );
}

} // namespace sake_unit_test

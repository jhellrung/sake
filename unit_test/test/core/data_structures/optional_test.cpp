/*******************************************************************************
 * unit_test/test/core/data_structures/optional_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/utility/nullptr.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void optional_test(sake::test::environment& env)
{
    SAKE_USING_NULLPTR;

    {
        sake::optional< int > x;
        SAKE_TEST_CHECK( env, !x.initialized() );
        SAKE_TEST_CHECK( env, !x );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
        x = 1;
        SAKE_TEST_REQUIRE( env, x.initialized() );
        SAKE_TEST_CHECK( env, x );
        SAKE_TEST_REQUIRE_RELATION( env, x.get_ptr(), !=, nullptr );
        SAKE_TEST_CHECK_RELATION( env, x.get(), ==, 1 );
        x = 2;
        SAKE_TEST_REQUIRE( env, x.initialized() );
        SAKE_TEST_REQUIRE_RELATION( env, x.get_ptr(), !=, nullptr );
        SAKE_TEST_CHECK_RELATION( env, x.get(), ==, 2 );
        x.reset();
        SAKE_TEST_CHECK( env, !x.initialized() );
        SAKE_TEST_CHECK_RELATION( env, x.get_ptr(), ==, nullptr );
    }
    {
    }
}

} // namespace sake_unit_test

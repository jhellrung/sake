/*******************************************************************************
 * unit_test/test/test/failure_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <cstdlib>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void failure_test(sake::test::environment& env)
{
    SAKE_TEST_WARN( env, false );
    SAKE_TEST_WARN_ALL( env, ( false ) ( true ) );
    SAKE_TEST_WARN_ALL( env, ( true ) ( false ) );
    SAKE_TEST_WARN_ANY( env, ( false ) ( false ) );
    SAKE_TEST_WARN_RELATION( env, 1, <, 0 );
    SAKE_TEST_WARN_RELATION_ALL( env, (( 1, <, 0 )) (( 1, <, 2 )) );
    SAKE_TEST_WARN_RELATION_ALL( env, (( 0, <, 1 )) (( 2, <, 1 )) );
    SAKE_TEST_WARN_RELATION_ANY( env, (( 1, <, 0 )) (( 2, <, 1 )) );

    SAKE_TEST_CHECK( env, false );
    SAKE_TEST_CHECK_ALL( env, ( false ) ( true ) );
    SAKE_TEST_CHECK_ALL( env, ( true ) ( false ) );
    SAKE_TEST_CHECK_ANY( env, ( false ) ( false ) );
    SAKE_TEST_CHECK_RELATION( env, 1, <, 0 );
    SAKE_TEST_CHECK_RELATION_ALL( env, (( 1, <, 0 )) (( 1, <, 2 )) );
    SAKE_TEST_CHECK_RELATION_ALL( env, (( 0, <, 1 )) (( 2, <, 1 )) );
    SAKE_TEST_CHECK_RELATION_ANY( env, (( 1, <, 0 )) (( 2, <, 1 )) );

    SAKE_TEST_REQUIRE( env, false );
    std::abort();
}

} // namespace sake_unit_test

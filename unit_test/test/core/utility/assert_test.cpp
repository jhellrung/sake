/*******************************************************************************
 * unit_test/test/core/utility/assert_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <iomanip>
#include <iosfwd>

#include <boost/preprocessor/seq/enum.hpp>

#include <sake/core/utility/assert.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#define SAKE_ASSERT_TEST_FAILURE_MACRO( data, info_seq ) \
    ++n_fail && \
    env.current_log_level() <= sake::test::environment::log_level_info && \
    ( sake::assert_failure_action::print( env.log(), data, BOOST_PP_SEQ_ENUM( info_seq ) ), true )

namespace sake_unit_test
{

void assert_test(sake::test::environment& env)
{
    // Expected successes.
    SAKE_REQUIRE( 0 < 1 && 1 < 2 );
    SAKE_REQUIRE( 1 < 0 || 1 < 2 );
    SAKE_REQUIRE_ALL( ( 0 < 1 ) ( 1 < 2 ) );
    SAKE_REQUIRE_ANY( ( 1 < 0 ) ( 1 < 2 ) );
    SAKE_REQUIRE_RELATION( 0, <, 1 );
    SAKE_REQUIRE_RELATION_ALL( (( 0, <, 1 )) (( 1, <, 2 )) );
    SAKE_REQUIRE_RELATION_ANY( (( 1, <, 0 )) (( 1, <, 2 )) );

    // Expected failures.
    unsigned int n_fail = 0;
    if(env.current_log_level() <= sake::test::environment::log_level_info)
        env.log() << std::setw(env.current_depth() + 1) << ""
                  << "Expected SAKE_REQUIRE*_TEST failures on lines "
                  << __LINE__ + 1 << " - " << __LINE__ + 7 << "..." << std::endl;
    SAKE_ASSERT_EXT( 0 < 1 && 2 < 1, SAKE_ASSERT_TEST_FAILURE_MACRO, "SAKE_REQUIRE_TEST" );
    SAKE_ASSERT_EXT( 1 < 0 || 2 < 1, SAKE_ASSERT_TEST_FAILURE_MACRO, "SAKE_REQUIRE_TEST" );
    SAKE_ASSERT_EXT_ALL( ( 0 < 1 ) ( 2 < 1 ), SAKE_ASSERT_TEST_FAILURE_MACRO, "SAKE_REQUIRE_ALL_TEST" );
    SAKE_ASSERT_EXT_ANY( ( 1 < 0 ) ( 2 < 1 ), SAKE_ASSERT_TEST_FAILURE_MACRO, "SAKE_REQUIRE_ANY_TEST" );
    SAKE_ASSERT_EXT_RELATION( 1, <, 0, SAKE_ASSERT_TEST_FAILURE_MACRO, "SAKE_REQUIRE_RELATION_TEST" );
    SAKE_ASSERT_EXT_RELATION_ALL( (( 0, <, 1 )) (( 2, <, 1 )), SAKE_ASSERT_TEST_FAILURE_MACRO, "SAKE_REQUIRE_RELATION_ALL_TEST" );
    SAKE_ASSERT_EXT_RELATION_ANY( (( 1, <, 0 )) (( 2, <, 1 )), SAKE_ASSERT_TEST_FAILURE_MACRO, "SAKE_REQUIRE_RELATION_ANY_TEST" );
    SAKE_TEST_CHECK_RELATION( env, n_fail, ==, 7 );
}

} // namespace sake_unit_test

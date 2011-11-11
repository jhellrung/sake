/*******************************************************************************
 * unit_test/test/core/math/binom_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/binom.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void binom_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::binom(0,0), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom(1,0), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(1,1), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom(2,0), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(2,1), ==, 2 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(2,2), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom(3,0), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(3,1), ==, 3 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(3,2), ==, 3 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(3,3), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom(4,0), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(4,1), ==, 4 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(4,2), ==, 6 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(4,3), ==, 4 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(4,4), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom(5,0), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(5,1), ==, 5 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(5,2), ==, 10 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(5,3), ==, 10 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(5,4), ==, 5 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(5,5), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom(6,0), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(6,1), ==, 6 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(6,2), ==, 15 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(6,3), ==, 20 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(6,4), ==, 15 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(6,5), ==, 6 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom(6,6), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<0>(0), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<0>(1), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<1>(1), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<0>(2), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<1>(2), ==, 2 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<2>(2), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<0>(3), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<1>(3), ==, 3 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<2>(3), ==, 3 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<3>(3), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<0>(4), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<1>(4), ==, 4 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<2>(4), ==, 6 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<3>(4), ==, 4 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<4>(4), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<0>(5), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<1>(5), ==, 5 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<2>(5), ==, 10 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<3>(5), ==, 10 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<4>(5), ==, 5 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<5>(5), ==, 1 );

    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<0>(6), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<1>(6), ==, 6 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<2>(6), ==, 15 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<3>(6), ==, 20 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<4>(6), ==, 15 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<5>(6), ==, 6 );
    SAKE_TEST_CHECK_RELATION( env, sake::binom_c<6>(6), ==, 1 );
}

} // namespace sake_unit_test

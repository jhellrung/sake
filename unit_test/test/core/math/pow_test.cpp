/*******************************************************************************
 * unit_test/test/core/math/pow_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/pow.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void pow_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::pow(0, 123456), ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow(1, 123456), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow(123456, 0), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow(123456, 1), ==, 123456 );

    SAKE_TEST_CHECK_RELATION( env, sake::pow_c< 123456 >(0), ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c< 123456 >(1), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<0>(123456), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<1>(123456), ==, 123456 );

    for(int n = 1, p = 0; p != 16; n *= 2, ++p)
        SAKE_TEST_CHECK_RELATION( env, sake::pow(2,p), ==, n );

    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<0>(2), ==,   1 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<1>(2), ==,   2 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<2>(2), ==,   4 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<3>(2), ==,   8 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<4>(2), ==,  16 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<5>(2), ==,  32 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<6>(2), ==,  64 );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<7>(2), ==, 128 );

    SAKE_TEST_CHECK_RELATION( env, sake::pow(1., -123456), ==, 1. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow(123456., -1), ==, 1/123456. );

    {
        double n = 1;
        for(int p = 0; p != -32; n /= 2, --p)
            SAKE_TEST_CHECK_RELATION( env, sake::pow(2.,p), ==, n );
    }

    SAKE_TEST_CHECK_RELATION( env, sake::pow_c< 0>(2.), ==, 1/  1. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<-1>(2.), ==, 1/  2. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<-2>(2.), ==, 1/  4. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<-3>(2.), ==, 1/  8. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<-4>(2.), ==, 1/ 16. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<-5>(2.), ==, 1/ 32. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<-6>(2.), ==, 1/ 64. );
    SAKE_TEST_CHECK_RELATION( env, sake::pow_c<-7>(2.), ==, 1/128. );
}

} // namespace sake_unit_test

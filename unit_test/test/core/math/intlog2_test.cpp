/*******************************************************************************
 * unit_test/test/core/math/intlog2_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/math/intlog2.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

void intlog2_test(sake::test::environment& env)
{
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(1), ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(2), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(3), ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(4), ==, 2 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(1000), ==, 9 );

    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.25), ==, -2 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.49), ==, -2 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.50), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.99), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(1.00), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(1.99), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(2.00), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(3.99), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(4.00), ==, +2 );

    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.25f), ==, -2 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.49f), ==, -2 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.50f), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(0.99f), ==, -1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(1.00f), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(1.99f), ==,  0 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(2.00f), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(3.99f), ==, +1 );
    SAKE_TEST_CHECK_RELATION( env, sake::intlog2(4.00f), ==, +2 );

    {
        bool thrown;
        try { thrown = false; sake::intlog2(0); }
        catch(sake::intlog2_error&) { thrown = true; }
        SAKE_TEST_CHECK( env, thrown );
        try { thrown = false; sake::intlog2(-1); }
        catch(sake::intlog2_error&) { thrown = true; }
        SAKE_TEST_CHECK( env, thrown );
        try { thrown = false; sake::intlog2(0.0); }
        catch(sake::intlog2_error&) { thrown = true; }
        SAKE_TEST_CHECK( env, thrown );
        try { thrown = false; sake::intlog2(-1.0); }
        catch(sake::intlog2_error&) { thrown = true; }
        SAKE_TEST_CHECK( env, thrown );
        try { thrown = false; sake::intlog2(0.0f); }
        catch(sake::intlog2_error&) { thrown = true; }
        SAKE_TEST_CHECK( env, thrown );
        try { thrown = false; sake::intlog2(-1.0f); }
        catch(sake::intlog2_error&) { thrown = true; }
        SAKE_TEST_CHECK( env, thrown );
    }
}

} // namespace sake_unit_test

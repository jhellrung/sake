/*******************************************************************************
 * unit_test/test/core/utility/address_of_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/utility/address_of.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

struct X { };

struct Y
{
    void operator&() const
    { }
};

} // namespace

void address_of_test(sake::test::environment& env)
{
    {
        int x = 0;
        SAKE_TEST_CHECK_RELATION( env, sake::address_of(x), ==, &x );
    }
    {
        X x;
        SAKE_TEST_CHECK_RELATION( env, sake::address_of(x), ==, &x );
    }
    {
        union { int x; Y y; } u;
        SAKE_TEST_CHECK_RELATION( env,
            static_cast< void* >(sake::address_of(u.y)),
            ==,
            static_cast< void* >(&u.x)
        );
    }
}

} // namespace sake_unit_test

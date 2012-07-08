/*******************************************************************************
 * unit_test/test/core/emplacer/emplacer_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <sake/core/emplacer/construct.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

struct X
{
    explicit X(int a_, int b_)
        : a(a_), b(b_)
    { }

    int a, b;
};

void emplacer_test(sake::test::environment& env)
{
    X x = sake::construct::emplacer(42, 43).construct<X>();
    SAKE_TEST_CHECK_RELATION( env, x.a, ==, 42 );
    SAKE_TEST_CHECK_RELATION( env, x.b, ==, 43 );
}

} // namespace sake_unit_test

/*******************************************************************************
 * unit_test/test/core/utility/direct_init_ctor_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/identity.hpp>

#include <sake/core/utility/direct_init_ctor.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

template< class T >
struct X
{
    SAKE_DIRECT_INIT_CTOR_DECLARE_MEMBERS( X,
        (( int, a ))
        (( class T, b ))
        (( typename boost::mpl::identity<T>::type, c ))
    )
};

} // namespace

void direct_init_ctor_test(sake::test::environment& env)
{
    X<int> x(0,1,2);
    SAKE_TEST_CHECK_RELATION( env, x.a, ==, 0 );
    SAKE_TEST_CHECK_RELATION( env, x.b, ==, 1 );
    SAKE_TEST_CHECK_RELATION( env, x.c, ==, 2 );
}

} // namespace sake_unit_test

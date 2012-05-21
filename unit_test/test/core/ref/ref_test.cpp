/*******************************************************************************
 * unit_test/test/core/ref/ref_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/set/set10.hpp>

#include <sake/core/ref/reference_wrapper.hpp>
#include <sake/core/ref/tag.hpp>
#include <sake/core/utility/nullptr.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

struct X
{
    typedef int result_type;
    int operator()() const
    { return 0; }
    int operator()(int) const
    { return 1; }
    int operator()(int, int) const
    { return 2; }
};

} // namespace

void ref_test(sake::test::environment& env)
{
    SAKE_USING_NULLPTR;

    X x;
    {
        sake::reference_wrapper<X> rx(x);
        SAKE_TEST_CHECK_RELATION( env, &rx.get(), ==, &x );
        SAKE_TEST_CHECK_RELATION( env, rx(), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, rx(0), ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, rx(0,1), ==, 2 );
    }
    {
        typedef boost::mpl::set1< sake::ref_tag::default_constructible > tags;
        sake::reference_wrapper< X, tags > rx;
        SAKE_TEST_CHECK_RELATION( env, rx.get_ptr(), ==, nullptr );
        rx = x;
        SAKE_TEST_CHECK_RELATION( env, &rx.get(), ==, &x );
        X y;
        rx = y;
        SAKE_TEST_CHECK_RELATION( env, &rx.get(), ==, &y );
    }
}

} // namespace sake_unit_test

/*******************************************************************************
 * unit_test/test/core/introspection/is_callable_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/assert.hpp>

#include <sake/core/introspection/is_callable.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace
{

BOOST_MPL_ASSERT_NOT((sake::is_callable< int >));

BOOST_MPL_ASSERT((sake::is_callable< void (*)( ) >));
BOOST_MPL_ASSERT((sake::is_callable< void (*)( ), void ( ) >));
BOOST_MPL_ASSERT((sake::is_callable< short (*)( long ), int ( int ) >));

struct no_callable { };
BOOST_MPL_ASSERT_NOT((sake::is_callable< no_callable >));

struct yes_callable0 { void operator()(); };
BOOST_MPL_ASSERT((sake::is_callable< yes_callable0 >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable0, void ( ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable0 const, void ( ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable0, void ( int ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable0, int ( ) >));

struct yes_callable1 { int operator()(int) const; };
BOOST_MPL_ASSERT((sake::is_callable< yes_callable1 >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable1, int ( int ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable1, void ( int ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable1, int ( short ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable1, long ( int ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable1 const, int ( int ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable1, void ( ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable1, void ( void* ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable1, void* ( int ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable1, void ( int, int ) >));

struct yes_callable2
{
    void* operator()(void*) const;
    int operator()(int, int);
};
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2 >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2, void* ( void* ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2, void ( void* ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2, void ( int* ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2 const, void* ( void* ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2, int ( int, int ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2, void ( int, int ) >));
BOOST_MPL_ASSERT((sake::is_callable< yes_callable2, long ( short, short ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable2, void ( ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable2, void ( int ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable2, int ( void* ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable2, void ( int, void* ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable2, void* ( int, int ) >));
BOOST_MPL_ASSERT_NOT((sake::is_callable< yes_callable2 const, int ( int, int ) >));

} // namespace

void is_callable_test(sake::test::environment& /*env*/)
{ }

} // namespace sake_unit_test

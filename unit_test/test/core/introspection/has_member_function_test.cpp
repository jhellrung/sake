/*******************************************************************************
 * unit_test/test/core/introspection/has_member_function_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/assert.hpp>

#include <sake/core/introspection/has_member_function.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace
{

#define SAKE_INTROSPECTION_TRAIT_NAME           has_mem_fn_f
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME f
#include SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION()

BOOST_MPL_ASSERT_NOT((has_mem_fn_f< int >));

struct no_f0 { };
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< no_f0 >));

class no_f1 { struct f; };
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< no_f1 >));
class no_f2 { static const int f = 0; };
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< no_f2 >));

struct yes_f0 { void f(); };
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f0 >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f0, void ( ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f0 const, void ( ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f0, int ( ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f0, void ( int ) >));

struct yes_f1 { int const & f() const; };
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f1 >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f1 const, int const & ( ) >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f1, int ( ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f1, void ( ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f1, int const & ( int ) >));

struct yes_f2 { void f(int); };
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f2 >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f2, void ( int ) >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f2, void ( short ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f2 const, void ( int ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f2, void ( ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f2, void ( void* ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f2, int ( int ) >));

struct yes_f3 { int f(int); };
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f3 >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f3, int ( int ) >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f3, void ( short ) >));
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f3, long ( short ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f3 const, int ( int ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f3, void ( ) >));
BOOST_MPL_ASSERT_NOT((has_mem_fn_f< yes_f3, void ( void* ) >));

class yes_f4 { void f(); };
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f4 >));

struct yes_f5 { struct f_ { void operator()(int); } f; };
BOOST_MPL_ASSERT((has_mem_fn_f< yes_f5 >));

} // namespace

void has_member_function_test(sake::test::environment& env)
{ }

} // namespace sake_unit_test

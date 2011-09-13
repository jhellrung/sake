/*******************************************************************************
 * unit_test/test/core/introspection/has_member_function_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/assert.hpp>

#include <sake/core/introspection/is_callable_member.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace
{

#define SAKE_INTROSPECTION_TRAIT_NAME  is_callable_member_x
#define SAKE_INTROSPECTION_MEMBER_NAME x
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER()

BOOST_MPL_ASSERT_NOT((is_callable_member_x< int >));

struct no_x0 { };
BOOST_MPL_ASSERT_NOT((is_callable_member_x< no_x0 >));

class no_x1 { struct x; };
BOOST_MPL_ASSERT_NOT((is_callable_member_x< no_x1 >));
class no_x2 { static const int x = 0; };
BOOST_MPL_ASSERT_NOT((is_callable_member_x< no_x2 >));
class no_x3 { template< class > struct x; };
BOOST_MPL_ASSERT_NOT((is_callable_member_x< no_x3 >));

struct yes_x0 { void x(); };
BOOST_MPL_ASSERT((is_callable_member_x< yes_x0 >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x0, void ( ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x0 const, void ( ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x0, int ( ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x0, void ( int ) >));

struct yes_x1 { int& x() const; };
BOOST_MPL_ASSERT((is_callable_member_x< yes_x1 >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x1 const, int& ( ) >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x1 const, int const & ( ) >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x1, int ( ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x1, void ( ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x1, int const & ( int ) >));

struct yes_x2 { void x(int); };
BOOST_MPL_ASSERT((is_callable_member_x< yes_x2 >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x2, void ( int ) >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x2, void ( short ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x2 const, void ( int ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x2, void ( ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x2, void ( void* ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x2, int ( int ) >));

struct yes_x3 { int x(int); };
BOOST_MPL_ASSERT((is_callable_member_x< yes_x3 >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x3, int ( int ) >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x3, void ( short ) >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x3, long ( short ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x3 const, int ( int ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x3, void ( ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x3, void ( void* ) >));

class yes_x4 { void x(); };
BOOST_MPL_ASSERT((is_callable_member_x< yes_x4 >));

struct yes_x5 { struct x_ { void operator()(int); } x; };
BOOST_MPL_ASSERT((is_callable_member_x< yes_x5 >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x5, void ( int ) >));
BOOST_MPL_ASSERT((is_callable_member_x< yes_x5, void ( short ) >));
//BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x5 const, void ( int ) >));
//BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x5, int ( void* ) >));
BOOST_MPL_ASSERT_NOT((is_callable_member_x< yes_x5, int ( int ) >));

} // namespace

void has_member_function_test(sake::test::environment& env)
{ }

} // namespace sake_unit_test
